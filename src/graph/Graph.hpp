#ifndef GRAPH_GRAPH_HPP_
#define GRAPH_GRAPH_HPP_

#include <types/NodeId.hpp>

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>


/*!
 * \brief Interface for a graph implementation.
 *
 * To avoid direct usage of a graph library (e.g. lemon or boost) this interface
 * is used as a wrapper to reduce needed code changes in case of a library switch.
 */
class IGraph
{
  public:
    /// Class specific exception type.
    using exception_type = std::runtime_error;

    /// Size type
    using size_type = std::size_t;

    /// Type for collection of nodes
    using node_collection_type = std::unordered_set<NodeId>;

    /// Get edges of a node.
    [[nodiscard]]
    virtual auto edges_of(NodeId node) const -> node_collection_type const& = 0;
   
    /// Get missing edges of a node.
    [[nodiscard]]
    virtual auto no_edges_of(NodeId node) const -> node_collection_type const& = 0;

    /// Create edge between two nodes.
    virtual auto connect(NodeId from, NodeId to) -> void = 0;

    /// Remove edge of two nodes.
    virtual auto disconnect(NodeId from, NodeId to) -> void = 0;

    /// Add a new node.
    virtual auto add(NodeId id) -> void = 0;

    /// Remove existing node.
    virtual auto remove(NodeId id) -> void = 0;
};

/// Use implementation: e.g using graph_to_use = Graph<MyGraphImplementation>;
template <typename G>
using Graph = std::enable_if_t<std::is_base_of_v<IGraph, G>, G>;

#endif

