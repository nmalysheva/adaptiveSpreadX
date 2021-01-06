#include <SSA.hpp>

#include <utils/Random.hpp>

#include <cmath>
#include <string>
#include <unordered_map>

#include <iostream>

SSA::SSA(double const start, double const end, ContactNetwork& network, Rules& rules) noexcept
    : m_now{start}, m_end{end}, m_network{network}, m_rules{rules}
{
}


auto SSA::execute() -> bool
{
    auto propensities = std::map<int, double>{};

    auto const edge_deletion_rates = m_network.get_edge_deletion_rates();
    if (not edge_deletion_rates.empty())
    {
        propensities.emplace(SSA::Edge_Delete, edge_deletion_rates.back().first);
    }

    auto const edge_creation_rates = m_network.get_edge_creation_rates();
    if (not edge_creation_rates.empty())
    {
        propensities.emplace(SSA::Edge_Create, edge_creation_rates.back().first);
    }


    auto birth_rates = std::vector<std::pair<double, std::string>>{};
    auto const& birth_rules = m_rules.get_birth_rules();
    auto last_rate = 0.0;
    for (auto const& birth : birth_rules)
    {
        birth_rates.emplace_back(static_cast<double>(birth.propability) + last_rate, birth.to_state);
        last_rate = birth_rates.back().first;
    }
    if (not birth_rates.empty())
    {
        propensities.emplace(SSA::Birth, birth_rates.back().first);
    }


    auto death_rates = std::vector<std::pair<double, NodeId>>{};
    auto const& death_rules = m_rules.get_death_rules();
    last_rate = 0.0;
    for (auto const& death : death_rules)
    {
        auto const affected_specie = m_network.get_specie(death.from_state);
        for (auto const& guy : affected_specie)
        {
            death_rates.emplace_back(static_cast<double>(death.propability) + last_rate, guy);
            last_rate = death_rates.back().first;
        }
    }
    
    if (not death_rates.empty())
    {
        propensities.emplace(SSA::Death, death_rates.back().first);
    }


    auto const& transition_rules = m_rules.get_transitions();
    auto Custom_Rule_Id = 0;
    for (auto const& transition : transition_rules)
    {
        auto const affected_specie = m_network.get_specie(transition.from_state);
        propensities[Custom_Rule_Id] = affected_specie.size() * static_cast<double> (transition.propability);
        ++Custom_Rule_Id;
    }
    const auto Transition_Rule_Max_Id = Custom_Rule_Id;

    //todo
    auto const& interaction_rules = m_rules.get_interactions();
    for (auto const& interaction : interaction_rules)
    {
        auto const affected_individuals = m_network.get_connections(interaction.from_state, interaction.connected_state);
        propensities[Custom_Rule_Id] = affected_individuals.size() * static_cast<double> (interaction.propability);
        ++Custom_Rule_Id;
    }

    const auto Interaction_Rule_Max_Id = Custom_Rule_Id;

    double propensities_sum = 0.0;
    for (auto const& it : propensities)
    {
        propensities_sum += it.second;
    }

    if (propensities_sum == 0.0)
    {
        return false;
    }

    double const r = random<double>();
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
            auto const detailed_sum = sum_r - sum;

            if (it.first == Edge_Create)
            {
                std::cout << "Create Edge" << std::endl;
                auto const result = SSA::find_action(edge_creation_rates, detailed_sum);
                m_network.create_edge(result->second.first, result->second.second);
            }
            else if (it.first == Edge_Delete)
            {
                std::cout << "Delete edge" << std::endl;
                auto const result = SSA::find_action(edge_deletion_rates, detailed_sum);
                m_network.delete_edge(result->second.first, result->second.second);
            }
            else if (it.first == Birth)
            {
                std::cout << "Birth" << std::endl;
                auto const result = SSA::find_action(birth_rates, detailed_sum);
                m_network.create(result->second);
            }
            else if (it.first == Death)
            {
                std::cout << "Death" << std::endl;
                auto const result = SSA::find_action(death_rates, detailed_sum);
                m_network.remove(result->second);
            }
            else if (it.first < Transition_Rule_Max_Id)
            {
                std::cout << "Transition" << std::endl;
                // custom rules
                // this only works if the prop of a custom rules is constant
                // e.g. S -> I always 0.5 and not drawn from a distribution
                auto const& the_rule = m_rules.get_transitions()[it.first];
                auto const chosen = static_cast<std::size_t> (detailed_sum / static_cast<double> (the_rule.propability));
                auto const affected_specie = m_network.get_specie(the_rule.from_state);
                m_network.change(/*node id of to-change-node*/ affected_specie[chosen], /*to_type*/ the_rule.to_state);
            }
            else if (it.first < Interaction_Rule_Max_Id)
            {
                //only works if the prop of an interaction is constant
                // e.g. S I -> I always 0.5 and not drawn from a distribution
                std::cout << "Interaction" << std::endl;
                auto const& the_rule = m_rules.get_interactions()[it.first];
                auto const chosen = static_cast<std::size_t> (detailed_sum / static_cast<double> (the_rule.propability));
                auto const affected_individuals = m_network.get_connections(the_rule.from_state, the_rule.connected_state);
                m_network.change(affected_individuals[chosen].first, the_rule.to_state);
            }
            else
            {
                throw "unknown rule";
            }
            break;
        }

        sum += it.second;
    }

    return true;
}

