#ifndef GRAPH_GRAPHIMPL_HPP_
#define GRAPH_GRAPHIMPL_HPP_

#include "Graph.hpp"

#include <types/NodeId.hpp>

#include <unordered_set>
#include <unordered_map>

/*!
 * \brief Basic implementation of IGraph.
 *
 * This is a very basic, not optimised, implementation of an undirected graph.
 * For faster and more efficient usage a proper graph library should be used.
 *
 * \note This implementation uses assert() to check for proper usage in debug mode.
 * In release mode no checks are performed.
 */
class GraphImpl final : public IGraph
{
  public:
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

