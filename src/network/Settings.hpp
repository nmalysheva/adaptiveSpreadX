#ifndef NETWORK_SETTINGS_HPP_
#define NETWORK_SETTINGS_HPP_

#include "Interaction.hpp"

#include <types/Distribution.hpp>
#include <types/State.hpp>
#include <types/EdgeModification.hpp>
#include <types/Transformation.hpp>
#include <types/Transition.hpp>

#include <cstddef>
#include <optional>
#include <set>


namespace network
{

/*!
 * \brief Store settings needed for ContactNetwork.
 *
 * Each `add_*` function takes care, that no duplicates are set and the states in the rules are available.
 *
 * \throws std::logic_error rule is not unique
 * \throws std::logic_error unknown state is used
 * \throws std::logic_error entry is defined twice
 *
 * \note At least one state must be added, but it is assumed that `settings::Settings` checks that that happens.
 */
class Settings final
{
  public:
    /// represents node initilisation (how many of which state)
    using NodeCount = Pair<std::size_t, State>;

    /// duplicate edges
    static constexpr auto DuplicateEdges = "duplice edge distribution";

    /// state already present
    static constexpr auto DuplicateState = "duplicate node initilisation";

    /// initial node count already set
    static constexpr auto DuplicateCount = "duplicate entry for node count initilisation";

    /// duplicate entry for edge count initilisation
    static constexpr auto DuplicateEdgeInit = "duplicate edge initilisation";

    /// unknown state
    static constexpr auto UnknownState = "unknown state";
    
    /// death distribution already set
    static constexpr auto DuplicateDeath = "duplicate death distribution";
    
    /// birth distribution already set
    static constexpr auto DuplicateBirth = "duplicate birth distribution";

    /// transition already set
    static constexpr auto DuplicateTransition = "duplicate transition";

    /// interaction already set
    static constexpr auto DuplicateInteraction = "duplicate interaction";


    /*!
     * \brief Add a new state.
     *
     * \throws std::logic_error state already used
     *
     * \param state the new state
     */
    auto add_state(State state) -> void;

    /// get collection of all defined states
    [[nodiscard]]
    auto states() const noexcept -> std::set<State> const&;

    /*!
     * \brief Add a distribution for creating an edge for a node of given state.
     *
     * \throws std::logic_error unknown state is used
     * \throws std::logic_error distribution already set
     *
     * \param s state of the node
     * \param d distribution for drawing the edge creation rate
     */
    auto add_edge_creation_distribution(State s, Distribution d) -> void;

    /// get collection of all defined edge creation distributions
    [[nodiscard]]
    auto edge_creation_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&;
    
    /*!
     * \brief Add a distribution for removing an edge for a node of given state.
     *
     * \throws std::logic_error unknown state is used
     * \throws std::logic_error distribution already set
     *
     * \param s state of the node
     * \param d distribution for drawing the edge removal rate
     */
    auto add_edge_removal_distribution(State s, Distribution d) -> void;
    
    /// get collection of all defined edge removal distributions
    [[nodiscard]]
    auto edge_removal_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&;

    /*!
     * \brief Add a distribution for the death of a node of given state.
     *
     * \throws std::logic_error unknown state is used
     * \throws std::logic_error distribution already set
     *
     * \param s state that will be removed
     * \param d distribution for drawing the death rate
     */
    auto add_death_distribution(State s, Distribution d) -> void;

    /// get collection of all defined death distributions
    [[nodiscard]]
    auto death_distributions() const noexcept -> std::set<DeathDistribution> const&;
    
    /*!
     * \brief Add a distribution for the birth of a node of given state.
     *
     * \throws std::logic_error unknown state is used
     * \throws std::logic_error distribution already set
     *
     * \param s state that will be created
     * \param d distribution for drawing the birth rate
     */
    auto add_birth_distribution(State s, Distribution d) -> void;

    /// get collection of all defined birth distributions
    [[nodiscard]]
    auto birth_distributions() const noexcept -> std::set<BirthDistribution> const&;

    /*!
     * \brief Add a node initilisation for the network.
     *
     * \throws std::logic_error unknown state is used
     * \throws std::logic_error count was already added
     * 
     * \param s state of node to create
     * \param count number of nodes (of state `s`) to create
     */
    auto add_node(State s, std::size_t count) -> void;

    /// return node initilisation
    [[nodiscard]]
    auto nodes() const noexcept -> std::set<NodeCount> const&;

    /*!
     * \brief Initilise network with `count` edges.
     *
     * \throws std::logic_error if called multiple times
     *
     * \param count number of initial edges
     */
    auto set_edges(std::size_t count) -> void;

    /// get number of edges to start with
    [[nodiscard]]
    auto edges() const noexcept -> std::size_t;
    
    /*!
     * \brief Add a `Transition` distribution
     *
     * \throws std::logic_error unknown states are used
     * \throws std::logic_error distribution already set
     *
     * \param f state that will be transformed
     * \param t resulting state
     * \param d propability distribution of this event
     */
    auto add_transition(State f, State t, Distribution d) -> void;
    
    /// return transitions
    [[nodiscard]]
    auto transitions() const noexcept -> std::set<TransitionDistribution> const&;

    /*!
     * \brief Add an `Interaction` distribution.
     *
     * \throws std::logic_error unknown states are used
     * \throws std::logic_error distribution already set
     *
     * \param f state that will be changed
     * \param c required connected state
     * \param t resulting state
     * \param d propability distribution of this event
     */
    auto add_interaction(State f, State c, State t, Distribution d) -> void;

    /// return interactions
    [[nodiscard]]
    auto interactions() const noexcept -> std::set<Interaction> const&;

  private:
    /// number of initial edges
    std::optional<std::size_t> m_edges{std::nullopt};

    /// defined states
    std::set<State> m_states{};

    /// defined edge creation distributions
    std::set<EdgeModificationDistribution> m_edge_creations{};

    /// defined edge removal distributions
    std::set<EdgeModificationDistribution> m_edge_removals{};

    /// node initilisation (count of state)
    std::set<NodeCount> m_nodes{};

    /// death distributions
    std::set<DeathDistribution> m_death_distributions{};

    /// birth distributions
    std::set<BirthDistribution> m_birth_distributions{};

    /// transition distributions
    std::set<TransitionDistribution> m_transitions;

    /// interaction distributions
    std::set<Interaction> m_interactions{};

    /// check if the state is known. Throws if not.
    auto check_state(State const& state) const -> void;
};

} // namespace settings

#endif

