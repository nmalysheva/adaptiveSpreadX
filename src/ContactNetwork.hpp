#ifndef CONTACTNETWORK_HPP_
#define CONTACTNETWORK_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <graph/GraphImpl.hpp>
#include <species/Individual.hpp>
#include <species/Species.hpp>
#include <types/NodeId.hpp>

#include <map>
#include <vector>
#include <utility>


class ContactNetwork final
{
  public:
    ContactNetwork() = delete;
    ContactNetwork(ConfigurationBlock const& config, Species const& s);

    auto print() const -> void;

    auto get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;
    auto get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;

    auto create_edge(NodeId const from, NodeId const to) -> void;
    auto delete_edge(NodeId const from, NodeId const to) -> void;

    /// Get all individuals of a given species (state)
    auto get_specie(std::string const& name) const -> std::vector<NodeId>;

    /// Get all edges that connect the given species (states)
    auto get_connections(std::string const& from, std::string const& to) const -> std::vector<std::pair<NodeId, NodeId>>;

    /// Change the state of a given node (by id)
    auto change(NodeId const& id, std::string const& to_state) -> void;

    /// Create a new node of given state
    auto create(std::string const& state) -> void;

    /// Remove given node
    auto remove(NodeId const id) -> void;

  private:
    Species const& m_species;
    std::map<NodeId, Individual> m_population;
    NodeId m_next_id;
    Graph<GraphImpl> m_graph;
};

#endif

