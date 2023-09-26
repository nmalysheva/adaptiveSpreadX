#ifndef ALGORITHM_ACTIONSFACTORY_HPP_
#define ALGORITHM_ACTIONSFACTORY_HPP_

#include "Actions.hpp"
#include <network/ContactNetwork.hpp>

#include <vector>


namespace algorithm
{

/*!
 * \brief The ActionsFactory can be used for faster access to the single Actions
 * or "group" of Actions (viral, contact, or both).
 *
 * It also reuses the memory allocated by all the Actions.
 * Further it offers the possibility to easily add edge actions (used by SSATAN-X).
 */
class ActionsFactory final
{
  public:
    /// Container type used
    using ActionPtrCollection = std::vector<Actions*>;

    /// Connect an ActionFactory and a network
    ActionsFactory(network::ContactNetwork& network) noexcept;

    /// Return all contact actions (ActionPtrCollection of delete and add edge events)
    auto get_contact_actions() -> ActionPtrCollection;

    /// Return all viral actions with given time (ActionPtrCollection of birth, death, transition, interaction events)
    auto get_viral_actions(double time) -> ActionPtrCollection;

    /// Return all actions ([contact, viral] expanded)
    auto get_all(double time) -> ActionPtrCollection;

    /// Add the last undo action that will re-add the latest removed edge
    auto add_undo_delete_edge_action(Actions& actions) const -> void;

    /// Add the last undo action that will re-delete the latest created edge
    auto add_undo_create_edge_action(Actions& actions) const -> void;
   
  private:
    /// set up the "delete edge" events
    auto get_delete_edge_actions() -> void;

    /// set up the "create edge" events
    auto get_create_edge_actions() -> void;

    /// set up the "remove a node" events 
    auto get_death_actions() -> void;

    /// set up the "create a node" events
    auto get_birth_actions(double time) -> void;
   
    /// set up the "spontaneous change state of a node" events
    auto get_transition_actions(double time) -> void;
   
    /// set up the "change state of a node, depending on its connections" events
    auto get_interaction_actions(double time) -> void;

    /// add a single edge deletion action
    auto add_single_deletion(Actions& actions, EdgeModificationRate const& params) const -> void;
    
    /// add a single edge creation action
    auto add_single_creation(Actions& actions, EdgeModificationRate const& params) const -> void;

    /// the network working on
    network::ContactNetwork& m_network;

    /// the delete edge actions
    Actions m_deletions{};

    /// the create edge actions
    Actions m_creations{};

    /// the remove node actions
    Actions m_deaths{};

    /// the create node actions
    Actions m_births{};

    /// the spontaneous change state of a node actions
    Actions m_transitions{};
    
    /// the change state of a node, depending on its connections actions
    Actions m_interactions{};
};

} // namespace algorithm

#endif

