#include "ActionsFactory.hpp"


namespace algorithm
{

namespace 
{
auto clean(Actions& actions, std::size_t const capacity) -> void
{
    actions.clear();
    actions.reserve(capacity);
}
} // namespace


ActionsFactory::ActionsFactory(network::ContactNetwork& network) noexcept
    : m_network{network}
{
}
    
auto ActionsFactory::get_contact_actions() -> ActionPtrCollection
{
    get_delete_edge_actions();
    get_create_edge_actions();
    return {&m_deletions, &m_creations};
}
    
auto ActionsFactory::get_viral_actions(double const time) -> ActionPtrCollection
{
    get_birth_actions(time);
    get_death_actions();
    get_transition_actions(time);
    get_interaction_actions(time);

    return {&m_births, &m_deaths, &m_transitions, &m_interactions};
}


auto ActionsFactory::get_all(double const time) -> ActionPtrCollection
{
    auto const contact = get_contact_actions();
    auto const viral = get_viral_actions(time);
    return {contact.front(), contact.back(), viral[0], viral[1], viral[2], viral[3]};
}


auto ActionsFactory::add_undo_create_edge_action(Actions& actions) const -> void
{
    auto const params = m_network.get_last_edge_undo();
    add_single_deletion(actions, params);
}


auto ActionsFactory::get_delete_edge_actions() -> void
{
    auto const& deletions = m_network.get_edge_deletion_rates();
    clean(m_deletions, deletions.size());

    for (auto const& deletion : deletions)
    {
        add_single_deletion(m_deletions, deletion);
    }
}


auto ActionsFactory::add_undo_delete_edge_action(Actions& actions) const -> void
{
    auto const params = m_network.get_last_edge_undo();
    add_single_creation(actions, params);
}


auto ActionsFactory::get_create_edge_actions() -> void
{
    auto const& creations = m_network.get_edge_creation_rates();
    clean(m_creations, creations.size());

    for (auto const& creation : creations)
    {
        add_single_creation(m_creations, creation);
    }
}


auto ActionsFactory::get_death_actions() -> void
{
    auto const& deaths = m_network.get_deaths();
    clean(m_deaths, deaths.size());
    
    auto death_action = [this] (auto const id) { this->m_network.remove(id); };

    for (auto const& death : deaths)
    {
        m_deaths.add(death.rate, death_action, death.identifier);
    }
}


auto ActionsFactory::get_birth_actions(double const time) -> void
{
    auto const& births = m_network.get_births();
    clean(m_births, births.size());

    auto birth_action = [this, time] (auto const& state) { this->m_network.create(time, state); };

    for (auto const& birth : births)
    {
        m_births.add(birth.rate, birth_action, birth.identifier);
    }
}


auto ActionsFactory::get_transition_actions(double const time) -> void
{
    auto const& transitions = m_network.get_transitions();
    clean(m_transitions, transitions.size());

    auto transition_action = [this, time] (auto const id, auto const state) { this->m_network.change(time, id, state); };

    for (auto const& transition : transitions)
    {
        m_transitions.add(transition.rate, transition_action, transition.from, transition.to);
    }
}


auto ActionsFactory::get_interaction_actions(double const time) -> void
{
    auto const& interactions = m_network.get_interactions();
    clean(m_interactions, interactions.size());

    auto interaction_action = [this, time] (auto const id, auto const state) { this->m_network.change(time, id, state); };

    for (auto const& interaction : interactions)
    {
        m_interactions.add(interaction.rate, interaction_action, interaction.from, interaction.to);
    }
}
    

auto ActionsFactory::add_single_deletion(Actions& actions, EdgeModificationRate const& params) const -> void
{
    auto const remove_action = [this] (auto const a, auto const b) { this->m_network.delete_edge(a, b); };
    actions.add(params.rate, remove_action, params.from, params.to);
}


auto ActionsFactory::add_single_creation(Actions& actions, EdgeModificationRate const& params) const -> void
{
    auto const create_action = [this] (auto const a, auto const b) { this->m_network.create_edge(a, b); };
    actions.add(params.rate, create_action, params.from, params.to);
}


} // network algorithm

