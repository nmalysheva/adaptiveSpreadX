#include "Settings.hpp"

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


auto Settings::add_state(State state) -> void
{
    validated_emplace(m_states, std::move(state), DuplicateState);
}


auto Settings::states() const noexcept -> std::set<State> const&
{
    return m_states;
}


auto Settings::add_edge_creation_distribution(State s, Distribution d) -> void
{
    check_state(s);
    validated_emplace(m_edge_creations, EdgeModificationDistribution{std::move(d), std::move(s)}, DuplicateEdges);
}


auto Settings::edge_creation_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&
{
    return m_edge_creations;
}


auto Settings::add_edge_removal_distribution(State s, Distribution d) -> void
{
    check_state(s);
    validated_emplace(m_edge_removals, EdgeModificationDistribution{std::move(d), std::move(s)}, DuplicateEdges);
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


auto Settings::add_node(State s, std::size_t const count) -> void
{
    check_state(s);
    validated_emplace(m_nodes, NodeCount{count, std::move(s)}, DuplicateCount);
}


auto Settings::nodes() const noexcept -> std::set<NodeCount> const&
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
    constexpr auto NoEdges = 0ul;
    return m_edges.value_or(NoEdges);
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

