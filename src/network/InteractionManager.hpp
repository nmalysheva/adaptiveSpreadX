#ifndef NETWORK_INTERACTIONMANAGER_HPP_
#define NETWORK_INTERACTIONMANAGER_HPP_

#include "Settings.hpp"
#include <types/NodeId.hpp>
#include <types/State.hpp>
#include <types/Transition.hpp>

#include <vector>


namespace network
{

/*!
 * \brief Manages possible interactions of the current state of the ContactNetwork.
 *
 * The interaction manager knows the interactions from the settings and has a list of
 * all connections of the network containing the possible state changes and rates.
 *
 * \note The list must be updated by the network if something changes.
 */
class InteractionManager final
{
  public:
    /// Initialise the object with the settings.
    InteractionManager(Settings const& settings);

    /// return the current interaction rates for the network
    [[nodiscard]]
    auto get_rates() const noexcept -> std::vector<InteractionRate> const&;

    /*!
     * \brief Get the resulting state of an interaction.
     *
     * Example: An interaction like "if A connected to B, A will turn to C"
     * the function-call `get_state(A, B)` will return `C`.
     *
     * \note The user has to take care that there is an interaction with the states available!
     * This is ensured if:
     * - the manager is kept up to date on network changes, and
     * - only states of the nodes with the ids from an entry returned by `get_rates` are used
     */
    [[nodiscard]]
    auto get_state(State const& from, State const& to) const -> State const&;

    /// remove all entries from rates that contain the given id (either connected from or to)
    auto remove(NodeId id) -> void;

    /// remove all entries from rates that contain both ids
    auto remove(NodeId from, NodeId to) -> void;

    /*!
     * \brief Add a new connection.
     *
     * The interaction rule for the connection "state_1 connected to state_2" is searched.
     * If found, the rate is drawn.
     * Then a new entry is stored in the rates container, containing the ids and the rate.
     *
     * Afterwards the same happens again, but for the rule "state_2 connected to state_1".
     *
     * \param first Id of the first node
     * \param second Id of the second node
     * \param state_1 state of the first node
     * \param state_2 state of the second node
     */
    auto add(NodeId first, NodeId second, State const& state_1, State const& state_2) -> void;

  private:
    /// interaction rules from the settings
    std::vector<Interaction> m_interactions{};

    /// collection of rates for the current state of the network
    std::vector<InteractionRate> m_rates{};

    /// performs one "search and insert" step as described in add
    auto add_impl(NodeId from, NodeId to, State const& from_state, State const& to_state) -> void;
};

} // namespace network

#endif

