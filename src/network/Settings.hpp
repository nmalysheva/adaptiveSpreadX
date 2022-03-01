#ifndef NETWORK_SETTINGS_HPP_
#define NETWORK_SETTINGS_HPP_

#include "Interaction.hpp"

#include <configuration/Configuration.hpp>
#include <types/Distribution.hpp>
#include <types/State.hpp>
#include <types/EdgeModification.hpp>
#include <types/Transformation.hpp>
#include <types/Transition.hpp>

#include <cstddef>
#include <map>
#include <optional>
#include <set>


namespace network
{

/*!
 * Contains all settings needed by the contact network.
 */
class Settings final
{
  public:
    /*!
     * \brief Store settings needed for ContactNetwork.
     *
     * Convert the corresponding entries from the given config to objects used by
     * the contact network.
     *
     * \throws std::logic_error required section is missing
     * \throws std::logic_error rule is not unique
     * \throws std::logic_error unknown state is used
     * \throws std::logic_error entry is defined twice
     * \throws any exception is thrown during parsing (e.g. of a Distribution)
     */
    Settings(configuration::Configuration const& config);

    /// Network section not present
    static constexpr auto NetworkMissing = "section [Network] missing";

    /// edge distribution already set
    static constexpr auto DuplicateEdge = "duplice edge distribution";

    /// state already present
    static constexpr auto DuplicateState = "duplicate node initilisation";

    /// duplicate entry for edge count initilisation
    static constexpr auto DuplicateEdgeInit = "duplicate edge initilisation";

    /// duplicate entry for seed initilisation
    static constexpr auto DuplicateSeedInit = "duplicate seed initilisation";

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

    /// quarantine already set
    static constexpr auto DuplicateQuarantine = "duplicate quarantine";

    /// default number of edges
    static constexpr auto DefaultEdges = 0ul;

    /// get collection of all defined states
    [[nodiscard]]
    auto states() const noexcept -> std::set<State> const&;

    /// get collection of all defined edge creation distributions
    [[nodiscard]]
    auto edge_creation_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&;
    
    /// get collection of all defined edge removal distributions
    [[nodiscard]]
    auto edge_removal_distributions() const noexcept -> std::set<EdgeModificationDistribution> const&;

    /// get collection of all defined death distributions
    [[nodiscard]]
    auto death_distributions() const noexcept -> std::set<DeathDistribution> const&;
    
    /// get collection of all defined birth distributions
    [[nodiscard]]
    auto birth_distributions() const noexcept -> std::set<BirthDistribution> const&;

    /// return node initilisation
    [[nodiscard]]
    auto nodes() const noexcept -> std::map<State, std::size_t> const&;

    /// get number of edges to start with
    [[nodiscard]]
    auto edges() const noexcept -> std::size_t;
    
    /// return transitions
    [[nodiscard]]
    auto transitions() const noexcept -> std::set<TransitionDistribution> const&;

    /// return interactions
    [[nodiscard]]
    auto interactions() const noexcept -> std::set<Interaction> const&;

    /// return seed
    [[nodiscard]]
    auto seed() const -> unsigned;

    /// return quarantines
    [[nodiscard]]
    auto quarantines() const noexcept -> std::map<State, FixedDistribution> const&;

  private:
    /// all known states
    std::set<State> m_states{};

    /// number of initial edges
    std::optional<std::size_t> m_edges{std::nullopt};

    /// seed
    std::optional<unsigned> m_seed{std::nullopt};

    /// defined edge creation distributions
    std::set<EdgeModificationDistribution> m_edge_creations{};

    /// defined edge removal distributions
    std::set<EdgeModificationDistribution> m_edge_removals{};

    /// node initilisation (count of state)
    std::map<State, std::size_t> m_nodes{};

    /// death distributions
    std::set<DeathDistribution> m_death_distributions{};

    /// birth distributions
    std::set<BirthDistribution> m_birth_distributions{};

    /// transition distributions
    std::set<TransitionDistribution> m_transitions;

    /// interaction distributions
    std::set<Interaction> m_interactions{};
    
    /// quarantine rules 
    std::map<State, FixedDistribution> m_quarantines{};

    /// check if the state is known. Throws if not.
    auto check_state(State const& state) const -> void;

    /*!
     * \brief Initilise network with `count` edges.
     *
     * \throws std::logic_error if called multiple times
     *
     * \param count number of initial edges
     */
    auto set_edges(std::size_t count) -> void;

    /*!
     * \brief Set the seed.
     *
     * \throws std::logic_error if called multiple times
     *
     * \param seed the seed
     */
    auto set_seed(unsigned seed) -> void;

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
    
    /*!
     * \brief Add a new state.
     *
     * \throws std::logic_error state already used
     *
     * \param state the new state
     */
    auto add_state(State state) -> void;

    /*!
     * \brief Add a new quarantine rule.
     *
     * \throws std::logic_error unknown states are used
     * \throws std::logic_error rule already set
     *
     * \param state the affected state
     * \param dist FixedDistribution that is used to draw the percentage of edges removed
     */
    auto add_quarantine(State state, FixedDistribution dist) -> void;
};

} // namespace settings

#endif

