#ifndef GRAPH_GRAPHIMPL_HPP_
#define GRAPH_GRAPHIMPL_HPP_

/*! \file 
 * \author Mathias Lindemann
 *
 * Contains simple implementation of the IGraph interface.
 */

#include <graph/Graph.hpp>
#include <types/NodeId.hpp>

#include <string>
#include <unordered_set>
#include <unordered_map>


/*!
 * \brief Basic implementation of IGraph.
 *
 * This is a very basic, not optimised, implementation of a directed graph.
 * For faster and more efficient usage a proper graph library should be used.
 *
 * \todo Make undirected
 */
class GraphImpl final : public IGraph
{
  public:
    /// Create an empty graph.
    GraphImpl() = default;

    /// \copydoc IGraph::num_nodes
    auto num_nodes() const -> size_type override;

    /// \copydoc IGraph::num_edges
    auto num_edges() const -> size_type override;

    /// \copydoc IGraph::print_edges
    auto print_edges() const -> void override;

    /// \copydoc IGraph::edges_of
    auto edges_of(NodeId const node) const -> node_collection_type const& override;

    /// \copydoc IGraph::no_edges_of
    auto no_edges_of(NodeId const node) const -> node_collection_type const& override;

    /// \copydoc IGraph::connect
    auto connect(NodeId const from, NodeId const to) -> void override;

    /// \copydoc IGraph::disconnect
    auto disconnect(NodeId const from, NodeId const to) -> void override;

    /// \copydoc IGraph::add
    auto add(NodeId const id) -> void override;

    /// \copydoc IGraph::remove
    auto remove(NodeId const id) -> void override;

  private:
    /// all connections
    std::unordered_map<NodeId, std::unordered_set<NodeId>> m_edges{};

    /// all unused connections
    std::unordered_map<NodeId, std::unordered_set<NodeId>> m_loose{};
};

#endif

