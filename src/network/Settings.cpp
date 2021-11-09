#include "Settings.hpp"

#include <utils/Parse.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>


namespace network
{

namespace
{
template <typename C>
auto validated_emplace(C& cont, typename C::value_type&& val, char const* const msg)
{
    auto const it = cont.emplace(std::move(val));
    if (not it.second)
    {
        throw std::logic_error{msg};
    }
}
} // namespace


Settings::Settings(configuration::Configuration const& config)
{
    auto const network = config.get("Network");
    if (not network)
    {
        throw std::logic_error{NetworkMissing};
    }

    for (auto const& entry : network->get())
    {
        auto const [name, value] = utils::parse::to_types<std::string, std::size_t>(entry);
        if (name == "edges")
        {
            set_edges(value);
        }
        else
        {
            add_node(utils::parse::to_type<State>(name), value);
        }
    }


    auto const births = config.get("Births");
    if (births)
    {
        for (auto const& entry : births->get())
        {
            auto [state, dist] = utils::parse::to_types<State, Distribution>(entry);
            add_birth_distribution(std::move(state), dist);
        }
    }

    auto const deaths = config.get("Deaths");
    if (deaths)
    {
        for (auto const& entry : deaths->get())
        {
            auto [state, dist] = utils::parse::to_types<State, Distribution>(entry);
            add_death_distribution(std::move(state), dist);
        }
    }

    auto const add_edges = config.get("AddEdges");
    if (add_edges)
    {
        for (auto const& entry : add_edges->get())
        {
            auto [state, dist] = utils::parse::to_types<State, Distribution>(entry);
            add_edge_creation_distribution(std::move(state), dist);
        }
    }

    auto const remove_edges = config.get("RemoveEdges");
    if (remove_edges)
    {
        for (auto const& entry : remove_edges->get())
        {
            auto [state, dist] = utils::parse::to_types<State, Distribution>(entry);
            add_edge_removal_distribution(std::move(state), dist);
        }
    }

    auto const transitions = config.get("Transitions");
    if (transitions)
    {
        for (auto const& entry : transitions->get())
        {
            auto [from, to, dist] = utils::parse::to_types<State, State, Distribution>(entry);
            add_transition(std::move(from), std::move(to), dist);
        }
    }

    auto const interactions = config.get("Interactions");
    if (interactions)
    {
        for (auto const& entry : interactions->get())
        {
            auto [from, connected, to, dist] = utils::parse::to_types<State, State, State, Distribution>(entry);
            add_interaction(std::move(from), std::move(connected), std::move(to), dist);
        }
    }
}

auto Settings::states() const noexcept -> std::set<State> const&
{
    return m_states;
}

auto Settings::add_node(State s, std::size_t const count) -> void
{
    m_states.emplace(s);
    validated_emplace(m_nodes, std::make_pair(std::move(s), count), DuplicateState);
}


auto Settings::add_edge_creation_distribution(State s, Distribution d) -> void
{
    check_state(s);
    validated_emplace(m_edge_creations, EdgeModificationDistribution{std::move(d), std::move(s)}, DuplicateEdge);
}


auto Settings::edge_creation_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&
{
    return m_edge_creations;
}


auto Settings::add_edge_removal_distribution(State s, Distribution d) -> void
{
    check_state(s);
    validated_emplace(m_edge_removals, EdgeModificationDistribution{std::move(d), std::move(s)}, DuplicateEdge);
}


auto Settings::edge_removal_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&
{
    return m_edge_removals;
}


auto Settings::add_death_distribution(State s, Distribution d) -> void
{
    check_state(s);
    validated_emplace(m_death_distributions, DeathDistribution{std::move(d), std::move(s)}, DuplicateDeath);
}


auto Settings::death_distributions() const noexcept -> std::set<DeathDistribution> const&
{
    return m_death_distributions;
}


auto Settings::add_birth_distribution(State s, Distribution d) -> void
{
    check_state(s);
    validated_emplace(m_birth_distributions, BirthDistribution{std::move(d), std::move(s)}, DuplicateBirth);
}


auto Settings::birth_distributions() const noexcept -> std::set<BirthDistribution> const&
{
    return m_birth_distributions;
}


auto Settings::nodes() const noexcept -> std::map<State, std::size_t> const&
{
    return m_nodes;
}


auto Settings::set_edges(std::size_t const count) -> void
{
    if (m_edges)
    {
        throw std::logic_error{DuplicateEdgeInit};
    }

    m_edges = count;
}


auto Settings::edges() const noexcept -> std::size_t
{
    return m_edges.value_or(DefaultEdges);
}


auto Settings::add_transition(State f, State t, Distribution d) -> void
{
    check_state(f);
    check_state(t);
    validated_emplace(m_transitions, TransitionDistribution{std::move(d), std::move(f), std::move(t)}, DuplicateTransition);
}


auto Settings::transitions() const noexcept -> std::set<TransitionDistribution> const&
{
    return m_transitions;
}


auto Settings::add_interaction(State f, State c, State t, Distribution d) -> void
{
    check_state(f);
    check_state(c);
    check_state(t);
    validated_emplace(m_interactions, Interaction{std::move(f), std::move(c), std::move(t), std::move(d)}, DuplicateInteraction);
}


auto Settings::interactions() const noexcept -> std::set<Interaction> const&
{
    return m_interactions;
}


auto Settings::check_state(State const& s) const -> void
{
    if (m_states.count(s) == 0)
    {
        throw std::logic_error{Settings::UnknownState};
    }
}

} // namespace settings

