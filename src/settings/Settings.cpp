#include "Settings.hpp"
#include "Parse.hpp"

#include <set>
#include <stdexcept>


namespace settings
{

namespace
{

template <typename... Ts>
auto validate(std::string&& header, std::set<State> const& base, Ts const&... states) -> void
{
    if ((... + base.count(states)) not_eq sizeof... (Ts))
    {
        throw std::logic_error{std::string{Settings::UnknownState} + std::move(header)};
    }
}
} // namespace

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
        m_network.add_factory(std::move(name), std::move(create), std::move(remove));
    } 

    auto const time_it = data.find("Time");
    if (time_it == data.end())
    {
        throw std::logic_error{Settings::NoTime};
    }

    auto const& times = time_it->second;
    if (times.size() not_eq 1)
    {
        throw std::logic_error{Settings::DuplicateTime};
    }

    auto const [time] = parse::to_types<double>(times.front());
    m_algorithm.set_time(time);

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
        else if (header == "Time")
        {
            continue; // already handled
        }
        else if (header == "Edges")
        {
            if (entries.size() not_eq 1)
            {
                throw std::logic_error{Settings::DuplicateEdges};
            }

            auto const [count] = parse::to_types<std::size_t>(entries.front());
            m_network.set_edges(count);
        }
        else if (header == "Nodes")
        {
            for (auto const& entry : entries)
            {
                auto [state, count] = parse::to_types<State, std::size_t>(entry);
                validate("Nodes", m_network.states(), state);
                m_network.add_node(std::move(state), count);
            }
        }
        else if (header == "Births")
        {
            for (auto const& entry : entries)
            {
                auto [state, dist] = parse::to_types<State, Distribution>(entry);
                validate("Births", m_network.states(), state);
                m_algorithm.add_birth(std::move(state), dist);
            }
        }
        else if (header == "Deaths")
        {
            for (auto const& entry : entries)
            {
                auto [state, dist] = parse::to_types<State, Distribution>(entry);
                validate("Deaths", m_network.states(), state);
                m_algorithm.add_death(std::move(state), dist);
            }
        }
        else if (header == "Transitions")
        {
            for (auto const& entry : entries)
            {
                auto [from, to, dist] = parse::to_types<State, State, Distribution>(entry);
                validate("Transitions", m_network.states(), from, to);
                m_algorithm.add_transition(std::move(from), std::move(to), dist);
            }
        }
        else if (header == "Interactions")
        {
            for (auto const& entry : entries)
            {
                auto [from, connected, to, dist] = parse::to_types<State, State, State, Distribution>(entry);
                validate("Interactions", m_network.states(), from, connected, to);
                m_algorithm.add_interaction(std::move(from), std::move(connected), std::move(to), dist);
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

