#include "SSATANX.hpp"

#include "ActionsFactory.hpp"
#include "SSA.hpp"
#include "TauLeap.hpp"
#include <utils/Random.hpp>
#include <types/DistributionImpl.hpp>

#include <cstddef>
#include <cmath>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <vector>


namespace algorithm
{

SSATANX::SSATANX(Settings const& settings, network::ContactNetwork& network)
    : Algorithm{settings, network}
{
}


auto SSATANX::run(utils::json::Block& json) -> void
{
    auto time = 0.0;
    auto thin = 0u;
    auto step = std::size_t{0};
    auto stats = utils::json::List<std::string>{};
    to_json(stats, time, thin);


    auto factory = ActionsFactory{m_network};
    auto tau_leap = TauLeap{m_network, m_settings.epsilon()};

    while (time < m_settings.time())
    {
        const auto t_lookahead = m_settings.time() - time;
        auto const btl = get_btl();
        if (btl == 0.0)
        {
            to_json(stats, time, thin);
            break;
        }
        const auto delta_t = ExponentialDistribution{btl}.draw();
        if (delta_t > t_lookahead)
        {
            //reject
            time = m_settings.time();
            to_json(stats, time, thin);
            break;
        }
        
        tau_leap.execute(time, time + delta_t);
        time += delta_t;
        auto viral_actions = factory.get_viral_actions(time);
    
        auto const action_sum = std::accumulate(viral_actions.begin(), viral_actions.end(), 0.0, [](auto const val, auto const& action)
                {
                return val + action->sum();
                });

        auto bound = utils::random_double(btl);

        if (action_sum >= bound)
        {
            // accept
            auto it = viral_actions.begin();
            while (bound > (*it)->sum())
            {
                bound -= (*it)->sum();
                it = std::next(it);
                assert(bound >= 0.0);
            }

            (*it)->call(bound);
        }
        else
        {
            //thin: network does not change (no unit-test)
            ++thin; // LCOV_EXCL_LINE
        }
        
        if (step == std::numeric_limits<std::size_t>::max())
        {
            // ensure no overflorw (no unit test)
            step = 0; // LCOV_EXCL_LINE
        }
        ++step;
        if (step == m_settings.output_step())
        {
            to_json(stats, time, thin);
            step = 0;
        }
    }

    json.add_json("networks", stats.to_string());
}


auto SSATANX::get_btl() -> double
{
    auto const deaths = m_network.get_deaths();
    auto btl = std::accumulate(deaths.cbegin(), deaths.cend(), 0.0, [](auto const val, auto const& death)
            {
                return val + death.rate;
            });
    auto const births = m_network.get_births();
    btl = std::accumulate(births.cbegin(), births.cend(), btl, [] (auto const val, auto const& birth)
            {
                return val + birth.rate;
            });

    auto const transitions = m_network.get_transitions();
    btl = std::accumulate(transitions.cbegin(), transitions.cend(), btl, [] (auto const val, auto const& transition)
            {
                return val + transition.rate;
            });

    auto const& state_counts = m_network.get_state_counts();
    auto const interaction_rates_max = m_network.get_max_interaction_rates();
    for (auto const& rate : interaction_rates_max)
    {
        btl += rate.rate * state_counts.at(rate.from) * state_counts.at(rate.to);
    }

    return btl;
}

} // namespace algorithm

