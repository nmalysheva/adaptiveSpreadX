#include "TauLeap.hpp"

#include "ActionsFactory.hpp"
#include "SSA.hpp"
#include <types/DistributionImpl.hpp>
#include <utils/Random.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <optional>


namespace algorithm
{

namespace
{

auto calculate_tau(Actions const& add, Actions const& remove, double const epsilon) -> std::optional<double>
{
    auto const variant = add.sum() + remove.sum();
    if (variant == 0.0)
    {
        return std::nullopt;
    }

    auto const edges = std::max(epsilon * remove.count(), 1.0);
    auto const edges_comp = std::max(epsilon * add.count(), 1.0);

    auto tau = std::min(std::pow(edges, 2), std::pow(edges_comp,2)) / variant;
    
    auto const expected = std::abs(add.sum() - remove.sum());
    if (expected == 0.0)
    {
        return tau; // LCOV_EXCL_LINE
    }
    
    auto const edge_comp_quot = edges_comp / expected;
    auto const edge_quot = edges / expected;

    return std::min({edge_comp_quot, edge_quot, tau});
}


auto leap_condition(unsigned const change, double const count, double const epsilon)
{
    return static_cast<double> (change) <= std::max(epsilon * count, 1.0);
}

} // namespace


TauLeap::TauLeap(network::ContactNetwork& network, double const epsilon)
    : m_factory{network}, m_epsilon{epsilon}
{
}


auto TauLeap::execute(double t_now, double const t_final) -> void
{
    // this must match the order of ActionsFactory::get_contact_actions
    constexpr auto remove_index = 0u;
    constexpr auto add_index = 1u;
    constexpr auto action_indices = std::array<unsigned, 2>{remove_index, add_index};

    auto actions = m_factory.get_contact_actions();

    auto const tau_opt = calculate_tau(*actions[remove_index], *actions[add_index], m_epsilon);
    if (not tau_opt)
    {
        return;
    }
    auto tau = tau_opt.value();

    auto C = std::array<unsigned, 2>{0, 0};
    auto T = std::array<double, 2>{0.0, 0.0};
    auto M = std::array<unsigned, 2>{0, 0};
    auto S = std::array<std::vector<std::pair<double, unsigned>>, 2>{};
    S[add_index] = {{0.0, 0}};
    S[remove_index] = {{0.0, 0}};
    auto row = std::array<unsigned, 2>{0u, 0u};

    while (t_now < t_final)
    {
        auto const delta_t = t_final - t_now;
        tau = std::min(tau, delta_t);
        
        auto const reactions_sum = actions[add_index]->sum() + actions[remove_index]->sum();
        auto const threshold = 10.0 / reactions_sum;
        if (tau < threshold)
        {
            constexpr auto SSA_runs = 100;
            for (int i = 0; i < SSA_runs; ++i)
            {
                auto const old_edge_count = actions[remove_index]->count();
                auto const proposed_time = SSA::step(delta_t, actions);
                if (not proposed_time)
                {
                    // no action can be performed
                    return; // LCOV_EXCL_LINE
                }
                t_now += proposed_time.value();
                actions = m_factory.get_contact_actions();
                
                assert(old_edge_count not_eq actions[remove_index]->count());
                if (old_edge_count < actions[remove_index]->count())
                {
                    // an edge was added
                    ++C[add_index];
                }
                else
                {
                    // an edge was deleted
                    ++C[remove_index];
                }
                
                for (auto const index : action_indices)
                {
                    T[index] += actions[index]->sum() * proposed_time.value();
                    auto const s_end = std::remove_if(S[index].begin(), S[index].end(), [T = T[index], C = C[index]](auto& val)
                        {
                            return (val.first < T) or (val.second < C);
                        });
                    S[index].erase(s_end, S[index].end());
                    S[index].emplace(S[index].begin(), T[index], C[index]);
                }
                
            }
            auto const tau_opt = calculate_tau(*actions[add_index], *actions[remove_index], m_epsilon);
            if (not tau_opt)
            {
                return; // LCOV_EXCL_LINE
            }
            tau = tau_opt.value();
        }
        else
        {
            for (auto const index : action_indices)
            {
                auto const& Sk = S[index];
                assert(not Sk.empty());
                auto& row_k = row[index];
                auto const r_t = actions[index]->sum() * tau + T[index];
                auto const lambda = r_t - Sk.back().first;
                if (lambda >= 0.0)
                {
                    M[index] = static_cast<unsigned> (PoissonDistribution{lambda}.draw()) + Sk.back().second - C[index];
                    row[index] = Sk.size() - 1;
                }
                else
                {
                    assert(Sk.size() > 1);
                    auto k = 1u;
                    for (; k < Sk.size(); ++k)
                    {
                        if ((Sk[k - 1].first <= r_t) and (r_t < Sk[k].first))
                        {
                            break;
                        }
                    }
                    assert(k not_eq Sk.size());
                    
                    row[index]  = k - 1;
                    auto const u = (r_t - Sk[row_k].first) / (Sk[k].first - Sk[row_k].first);
                    M[index] = static_cast<unsigned> (BinomialDistribution{Sk[k].second - Sk[row_k].second, u}.draw()) + Sk[row_k].second - C[index];
                }
                assert(M[index] >= 0.0);
            }
           
            auto add_lc = leap_condition(M[add_index], actions[remove_index]->count(), m_epsilon);
            auto remove_lc = leap_condition(M[remove_index], actions[add_index]->count(), m_epsilon);
            if (add_lc and remove_lc)
            {
                t_now += tau;

                for (auto const index : action_indices)
                {
                    assert(row[index] < S[index].size());
                    S[index].erase(S[index].begin(), std::next(S[index].begin(), row[index] + 1));
                    T[index] += actions[index]->sum() * tau;
                    C[index] += M[index];
                    S[index].emplace(S[index].begin(), T[index], C[index]);
                }


                constexpr auto scale = 0.75;
                add_lc = leap_condition(M[add_index], actions[remove_index]->count(), scale * m_epsilon);
                remove_lc = leap_condition(M[remove_index], actions[add_index]->count(), scale * m_epsilon);
                if (not (add_lc and remove_lc))
                {
                    constexpr auto p_star = 0.9;
                    tau *= p_star;
                }
                else
                {
                    constexpr auto q = 0.98;
                    constexpr auto q_star = 1.02;
                    const auto expo = t_now < 1.0 ? q : q_star;
                    tau = std::pow(tau, expo);
                }

                //bulk update
                auto types = std::vector<unsigned>(M[add_index] + M[remove_index], add_index);
                std::fill_n(types.begin(), M[remove_index], remove_index);
                auto g = std::mt19937{std::random_device{}()};
                std::shuffle(types.begin(), types.end(), g);

                for (auto const index : types)
                {
                    auto const bound = utils::random_double(actions[index]->sum());
                    actions[index]->call_and_remove(bound);
                    
                    if (index == add_index)
                    {
                        m_factory.add_undo_create_edge_action(*actions[remove_index]);
                    }
                    else
                    {
                        m_factory.add_undo_delete_edge_action(*actions[add_index]);
                    }
                }
                actions = m_factory.get_contact_actions();
            }
            else
            {
                for (auto const index : action_indices)
                {
                    assert(row[index] < S[index].size());
                    auto const where = std::next(S[index].begin(), row[index] + 1);
                    S[index].emplace(where, actions[index]->sum() * tau + T[index], C[index] + M[index]);
                }

                constexpr double p = 0.75;
                tau *= p;
            }
        }
    }
}

} // namespace algorithm

