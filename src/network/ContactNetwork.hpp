#ifndef CONTACTNETWORK_HPP_
#define CONTACTNETWORK_HPP_

#include <graph/GraphImpl.hpp>
#include <settings/Network.hpp>

#include <cstddef>
#include <map>
#include <set>
#include <vector>
#include <utility>


namespace network
{

class ContactNetwork final
{
  public:
    ContactNetwork(settings::Network const& settings);

    auto get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;
    auto get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;

    auto create_edge(NodeId const from, NodeId const to) -> void;
    auto delete_edge(NodeId const from, NodeId const to) -> void;

    /// Get all individuals of a given species (state)
    auto get_specie(State const& state) const -> std::vector<NodeId>;
    auto count_specie(State const& state) const -> std::size_t;

    /// Get all edges that connect the given species (states)
    auto get_connections(State const& from, State const& to) const -> std::vector<std::pair<NodeId, std::size_t>>;

    /// Change the state of a given node (by id)
    auto change(NodeId const& id, State const& to_state) -> void;

    /// Create a new node of given state
    auto create(State const& state) -> void;

    /// Remove given node
    auto remove(NodeId const id) -> void;

    auto get_all() const noexcept
    {
        return m_population;
    }

  private:
    std::map<State, IndividualFactory> m_species;
    std::map<NodeId, Individual> m_population{};
    std::map<State, std::set<NodeId>> m_detailed_population{};
    Graph<GraphImpl> m_graph{};
};

}

#endif

