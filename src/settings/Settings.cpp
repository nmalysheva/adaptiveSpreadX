#include "Settings.hpp"

#include <set>

#include <utils/Parse.hpp>


namespace settings
{

namespace
{

template <typename... Ts>
auto validate(std::set<State> const& base, Ts const&... states) -> void
{
    if ((... + base.count(states)) not_eq sizeof... (Ts))
    {
        throw;
    }
}
} // namespace

Settings::Settings(std::unordered_map<std::string, std::vector<std::string>> const& data)
{
    auto const species_it = data.find("Species");
    if (species_it == data.end())
    {
        throw "no species defined";
    }

    auto species = std::set<State>{};
    for (auto const& entry : species_it->second)
    {
        auto [name, create, remove]  = parse::to_types<State, Distribution, Distribution>(entry);
        if (not species.emplace(name).second)
        {
            throw "species defined twice";
        }
        if (not m_network.add_factory(name, create, remove))
        {
            throw "cannot happen...";
        }
    } 

    auto const time_it = data.find("Time");
    if (time_it == data.end())
    {
        throw "time not defined";
    }

    auto const& times = time_it->second;
    if (times.size() not_eq 1)
    {
        throw "time only 1 entry allowed";
    }

    auto const [time] = parse::to_types<double>(times.front());
    m_algorithm.set_time(time);


    for (auto const& [header, entries] : data)
    {
        if (entries.empty())
        {
            throw "section ... contains no data";
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
                throw "edges only 1 entry allowed";
            }

            auto const [count] = parse::to_types<std::size_t>(entries.front());
            m_network.set_edges(count);
        }
        else if (header == "Nodes")
        {
            for (auto const& entry : entries)
            {
                auto [state, count] = parse::to_types<State, std::size_t>(entry);
                validate(species, state);
                m_network.add_node(std::move(state), count);
            }
        }
        else if (header == "Births")
        {
            for (auto const& entry : entries)
            {
                auto [state, dist] = parse::to_types<State, Distribution>(entry);
                validate(species, state);
                m_algorithm.add_birth(std::move(state), dist);
            }
        }
        else if (header == "Deaths")
        {
            for (auto const& entry : entries)
            {
                auto [state, dist] = parse::to_types<State, Distribution>(entry);
                validate(species, state);
                m_algorithm.add_death(std::move(state), dist);
            }
        }
        else if (header == "Transitions")
        {
            for (auto const& entry : entries)
            {
                auto [from, to, dist] = parse::to_types<State, State, Distribution>(entry);
                validate(species, from, to);
                m_algorithm.add_transition(std::move(from), std::move(to), dist);
            }
        }
        else if (header == "Interactions")
        {
            for (auto const& entry : entries)
            {
                auto [from, connected, to, dist] = parse::to_types<State, State, State, Distribution>(entry);
                validate(species, from, connected, to);
                m_algorithm.add_interaction(std::move(from), std::move(connected), std::move(to), dist);
            }
        }
        else
        {
            throw "unknown section";
        } 
    }
}


auto Settings::network() const noexcept -> Network const&
{
    return m_network;
}


auto Settings::algorithm() const noexcept -> Algorithm const&
{
    return m_algorithm;
}

} // namespace settings

