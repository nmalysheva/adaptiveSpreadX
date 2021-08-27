#ifndef NETWORK_CONTACTNETWORK_HPP_
#define NETWORK_CONTACTNETWORK_HPP_

#include "Individual.hpp"
#include "IndividualFactory.hpp"
#include "NodeId.hpp"
#include "Settings.hpp"

#include <graph/GraphImpl.hpp>

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>


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
     * The rate is the maximum of the individual's remove rates.
     */
    auto get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<node_type, node_type>>>;

    /*!
     * \brief Get all edge creation rates.
     *
     * \return a collection of two unconnected nodes and a rate that a connection will happen.
     * The rate is the minimum of the individual's connection rates.
     */
    auto get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<node_type, node_type>>>;

    /// create edge between two nodes
    auto create_edge(node_type from, node_type to) -> void;

    /// delete edge of two nodes
    auto delete_edge(node_type from, node_type to) -> void;

    /// get all ids of individuals with given state
    auto get_specie(State const& state) const -> std::set<node_type> const&;

    /// count all individuals with given state
    auto count_specie(State const& state) const -> std::size_t;

    /// get all edges that connect nodes with given state
    auto get_connections(State const& from, State const& to) const -> std::vector<std::pair<node_type, std::size_t>>;

    /// change the state of a given node (by id)
    auto change(node_type const& id, State const& to_state) -> void;

    /// create a new node of given state
    auto create(State const& state) -> void;

    /// remove given node
    auto remove(node_type id) -> void;

    /// return json representation of this network
    auto to_json() const -> std::string;

  private:
    /// factory for individual creation
    IndividualFactory m_species;

    /// the whole population
    std::map<node_type, Individual> m_population{};

    /// detailed population: state -> list of ids
    std::map<State, std::set<node_type>> m_detailed_population{};

    /// used graph
    Graph<GraphImpl> m_graph{};
};

}

#endif

