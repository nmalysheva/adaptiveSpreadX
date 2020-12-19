#include <SSA.hpp>

#include <unordered_map>
#include <string>

#include <random>
#include <Utility.h>
#include <cmath>


SSA::SSA(double const start, double const end, ContactNetwork& network) noexcept
    : m_now{start}, m_end{end}, m_network{network}
{
}


auto SSA::execute() -> bool
{
    std::unordered_map<std::string, double> propensities{{"edge_delete", 0}, {"edge_create", 0}};

    auto const edge_deletion_rates = m_network.get_edge_deletion_rates();
    propensities["edge_delete"] = edge_deletion_rates.back().first;

    auto const edge_creation_rates = m_network.get_edge_creation_rates();
    propensities["edge_create"] = edge_creation_rates.back().first;

    double propensities_sum = 0.0;
    for (auto const& it : propensities)
    {
        propensities_sum += it.second;
    }

    if (propensities_sum == 0.0)
    {
        return false;
    }

    auto generator = std::mt19937_64{std::random_device{}()};
    double const r = sampleRandUni(generator);
    double const proposed_time = 1.0 / propensities_sum * std::log(1.0 / r);


    m_now += proposed_time;
    if (m_now > m_end)
    {
        return false;
    }

    double sum = 0.0;
    auto const sum_r = propensities_sum * r;

    for (auto const& it : propensities)
    {
        if (sum + it.second >= sum_r)
        {
            if (it.first == "edge_create")
            {
                for (auto const& edge_info : edge_creation_rates)
                {
                    if (sum_r - sum <= edge_info.first)
                    {
                        m_network.create_edge(edge_info.second.first, edge_info.second.second);
                        break;
                    }
                }
            }
            else if (it.first == "edge_delete")
            {
                for (auto const& edge_info : edge_deletion_rates)
                {
                    if (sum_r - sum <= edge_info.first)
                    {
                        m_network.delete_edge(edge_info.second.first, edge_info.second.second);
                        break;
                    }
                }
            }
            else
            {
                throw "INCORRECT ACTION";
            }
            break;
        }
        sum += it.second;
    }

    return true;
}

