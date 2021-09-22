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
    using node_type = NodeId;

    /// initilise with given settings.
    ContactNetwork(Settings const& settings);

    /*!
     * \brief Get all edge deletion rates.
     *
     * \return a collection of two connected nodes and a rate that a connection will be removed.
     * The rate is the product of the individual's remove rates.
     */
    auto get_edge_deletion_rates() const -> std::vector<EdgeModificationRate>;

    /*!
     * \brief Get all edge creation rates.
     *
     * \return a collection of two unconnected nodes and a rate that a connection will happen.
     * The rate is the product of the individual's connection rates.
     */
    auto get_edge_creation_rates() const -> std::vector<EdgeModificationRate>;


    /// return possible death events
    [[nodiscard]]
    auto get_deaths() const -> std::vector<DeathRate>;

    /// return possible birth events
    auto get_births() const -> std::vector<BirthRate>;

    /// return possible transition events
    auto get_transitions() const -> std::vector<TransitionRate>;

    /// return possible interaction events
    auto get_interactions() const -> std::vector<TransitionRate>;

    /// create edge between two nodes
    auto create_edge(node_type from, node_type to) -> void;

    /// delete edge of two nodes
    auto delete_edge(node_type from, node_type to) -> void;

    /// change the state of a given node (by id) (and store simulation timestamp)
    auto change(double simulation_time, node_type const& id, State const& to_state) -> void;

    /// create a new node of given state (and store simulation timestamp)
    auto create(double simulation_time, State const& state) -> void;

    /// remove given node
    auto remove(node_type id) -> void;

    /// return json representation of this network
    auto to_json() const -> std::string;

  private:
    /// factory for individual creation
    IndividualFactory m_species_factory;

    /// the whole population
    std::map<node_type, Individual> m_population{};

    /// used graph
    Graph<GraphImpl> m_graph{};

    /// stores and manages the interactions
    InteractionManager m_interaction_manager;
};

}

#endif

