#ifndef NETWORK_CONTACTNETWORK_HPP_
#define NETWORK_CONTACTNETWORK_HPP_

#include "Individual.hpp"
#include "IndividualFactory.hpp"
#include "InteractionManager.hpp"
#include "Settings.hpp"

#include <graph/GraphImpl.hpp>
#include <types/NodeId.hpp>
#include <types/EdgeModification.hpp>
#include <types/Transition.hpp>
#include <types/Transformation.hpp>

#include <map>
#include <vector>


namespace network
{

/*!
 * \brief Representation and manager of a network for the algorithm to work with.
 *
 * \note The ContactNetwork assumes that `Settings` and all function input values are valid.
 */
class ContactNetwork final
{
  public:
    /// typedef for the used node_type
    using node_type = NodeId;

    /// initilise with given settings.
    ContactNetwork(Settings const& settings);

    /// only one network per run
    ContactNetwork(ContactNetwork const&) = delete;
    
    /// only one network per run
    auto operator=(ContactNetwork const&) -> ContactNetwork& = delete;

    /*!
     * \brief Get all edge deletion rates.
     *
     * \return a collection of two connected nodes and a rate that a connection will be removed.
     * The rate is the product of the individual's remove rates.
     */
    [[nodiscard]]
    auto get_edge_deletion_rates() -> std::vector<EdgeModificationRate> const&;

    /*!
     * \brief Get all edge creation rates.
     *
     * \return a collection of two unconnected nodes and a rate that a connection will happen.
     * The rate is the product of the individual's connection rates.
     */
    [[nodiscard]]
    auto get_edge_creation_rates() -> std::vector<EdgeModificationRate> const&;

    /// return possible death events
    [[nodiscard]]
    auto get_deaths() -> std::vector<DeathRate> const&;

    /// return possible birth events
    [[nodiscard]]
    auto get_births() const -> std::vector<BirthRate> const&;

    /// return possible transition events
    [[nodiscard]]
    auto get_transitions() -> std::vector<TransitionRate> const&;

    /// return possible interaction events
    [[nodiscard]]
    auto get_interactions() -> std::vector<TransitionRate> const&;

    /// create edge between two nodes
    auto create_edge(node_type from, node_type to) -> void;

    /// delete edge of two nodes
    auto delete_edge(node_type from, node_type to) -> void;

    /*!
     * \brief Change the state of a given node (by id).
     *
     * Changing the state of a node consists of several steps:
     * 1 Changes the state of the node and updates its modification time.
     * 2 Update the possible interactions.
     * 3 Perform adaption rules
     *
     * \param simulation_time modification time of the node
     * \param id the id of the node to change
     * \param to_state the new state
     */
    auto change(double simulation_time, node_type id, State to_state) -> void;

    /// create a new node of given state (and store simulation timestamp)
    auto create(double simulation_time, State const& state) -> void;

    /// remove given node
    auto remove(node_type id) -> void;

    /// return json representation of this network
    auto to_json() const -> std::string;

    /// get mapping of "available state (as set in the configuration) -> how many are in the network"
    [[nodiscard]]
    auto get_state_counts() const noexcept -> std::map<State, std::size_t> const&;

    /// get the maximum rates of all interactions (according to the used configuration)
    [[nodiscard]]
    auto get_max_interaction_rates() const noexcept -> std::vector<StateTransitionRate>;

    /*!
     * \brief Get the "undo" event for the last edge change.
     *
     * If the last edge change was a remove, an add event is returned.
     * If the last edge change was an add, a remove event is returned.
     *
     * The rates are calculated the same way as in get_edge_creation_rates / get_edge_deletion_rates.
     *
     * \note If no edge change was performed so far this function must not be called (throws std::bad_optional_access).
     * \note If after an edge change the affected nodes change, this function must not be called (incorrect values are returned).
     */
    [[nodiscard]]
    auto get_last_edge_undo() const -> EdgeModificationRate;

  private:
    /// factory for individual creation
    IndividualFactory m_species_factory;

    /// the whole population
    std::map<node_type, Individual> m_population{};

    /// used graph
    Graph<GraphImpl> m_graph{};

    /// stores and manages the interactions
    InteractionManager m_interaction_manager;

    /// State database
    std::map<State, std::size_t> m_state_db{};

    std::optional<EdgeModificationRate> m_last_egdge_undo{};

    /// edge addition rate storage
    std::vector<EdgeModificationRate> m_edge_create_rates{};

    /// edge deletion rate storage
    std::vector<EdgeModificationRate> m_edge_remove_rates{};

    /// death rate storage
    std::vector<DeathRate> m_death_rates{};

    /// birth rate storage <- not needed, stored by m_species_factory
    //std::vector<BirthRate> m_birth_rates{};

    /// transition rate storage
    std::vector<TransitionRate> m_transition_rates{};
    
    /// interaction rate storage
    std::vector<TransitionRate> m_interaction_rates{};

    /// adaption rules
    std::map<State, std::vector<AdaptionData>> const m_adaptions;

    /// change the state of the node and update the interactions
    auto change_state(double simulation_time, node_type const& id, State const& to_state) -> void;
};

}

#endif

