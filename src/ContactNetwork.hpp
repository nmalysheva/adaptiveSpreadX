#ifndef CONTACTNETWORK_HPP_
#define CONTACTNETWORK_HPP_

#include <graph/GraphImpl.hpp>
#include <network/Individual.hpp>
#include <network/Settings.hpp>
#include <types/State.hpp>
#include <types/NodeId.hpp>

#include <map>
#include <vector>
#include <utility>


namespace network
{

class ContactNetwork final
{
  public:
    ContactNetwork() = delete;
    ContactNetwork(Settings const& settings);

    auto print() const -> void;

    auto get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;
    auto get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;

    auto create_edge(NodeId const from, NodeId const to) -> void;
    auto delete_edge(NodeId const from, NodeId const to) -> void;

    /// Get all individuals of a given species (state)
    auto get_specie(State const& state) const -> std::vector<NodeId>;

    /// Get all edges that connect the given species (states)
    auto get_connections(State const& from, State const& to) const -> std::vector<std::pair<NodeId, NodeId>>;

    /// Change the state of a given node (by id)
    auto change(NodeId const& id, State const& to_state) -> void;

    /// Create a new node of given state
    auto create(State const& state) -> void;

    /// Remove given node
    auto remove(NodeId const id) -> void;

  private:
    IndividualFactories const m_species;
    std::map<NodeId, Individual> m_population{};
    Graph<GraphImpl> m_graph{};
};

}

#endif

