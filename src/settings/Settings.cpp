#include "Settings.hpp"
#include "Parse.hpp"

#include <set>
#include <stdexcept>


namespace settings
{

Settings::Settings(std::unordered_map<std::string, std::vector<std::string>> const& data)
{
    auto const species_it = data.find("Species");
    if (species_it == data.end())
    {
        throw std::logic_error{Settings::NoSpecies};
    }

    for (auto const& entry : species_it->second)
    {
        auto [name, create, remove]  = parse::to_types<State, Distribution, Distribution>(entry);
        m_network.add_state(name);
        m_network.add_edge_creation_distribution(name, create);
        m_network.add_edge_removal_distribution(name, remove);
    } 

    auto const time_it = data.find("Time");
    if (time_it == data.end())
    {
        throw std::logic_error{Settings::NoTime};
    }

    for (auto const& entry : time_it->second)
    {
        auto [value] = parse::to_types<double>(entry);
        m_algorithm.set_time(value);
    }

    
    for (auto const& [header, entries] : data)
    {
        if (entries.empty())
        {
            throw std::logic_error{std::string{Settings::EmptySection} + header};
        }

        if (header == "Species")
        {
            continue; // already handled
        }
        
        if (header == "Time")
        {
            continue; // already handled
        }

        if (header == "Edges")
        {
            auto const [count] = parse::to_types<std::size_t>(entries.front());
            m_network.set_edges(count);
        }
        else if (header == "Nodes")
        {
            for (auto const& entry : entries)
            {
                auto [state, count] = parse::to_types<State, std::size_t>(entry);
                m_network.add_node(std::move(state), count);
            }
        }
        else if (header == "Births")
        {
            for (auto const& entry : entries)
            {
                auto [state, dist] = parse::to_types<State, Distribution>(entry);
                m_network.add_birth_distribution(std::move(state), dist);
            }
        }
        else if (header == "Deaths")
        {
            for (auto const& entry : entries)
            {
                auto [state, dist] = parse::to_types<State, Distribution>(entry);
                m_network.add_death_distribution(state, dist);
            }
        }
        else if (header == "Transitions")
        {
            for (auto const& entry : entries)
            {
                auto [from, to, dist] = parse::to_types<State, State, Distribution>(entry);
                m_network.add_transition(std::move(from), std::move(to), dist);
            }
        }
        else if (header == "Interactions")
        {
            for (auto const& entry : entries)
            {
                auto [from, connected, to, dist] = parse::to_types<State, State, State, Distribution>(entry);
                m_network.add_interaction(std::move(from), std::move(connected), std::move(to), dist);
            }
        }
        else if (header == "Output")
        {
            for (auto const& entry : entries)
            {
                auto [value] = parse::to_types<std::size_t>(entry);
                m_algorithm.set_output_step(value);
            }
        }
        else
        {
            throw std::logic_error{std::string{Settings::UnknownSection} + header};
        } 
    }
}


auto Settings::network() const noexcept -> network::Settings const&
{
    return m_network;
}


auto Settings::algorithm() const noexcept -> algorithm::Settings const&
{
    return m_algorithm;
}

} // namespace settings

