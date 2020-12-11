#ifndef GRAPH_GRAPH_HPP_
#define GRAPH_GRAPH_HPP_

#include <cstddef>
#include <stdexcept>
#include <type_traits>

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

    IGraph() = default;

    /// Get number of nodes.
    [[nodiscard]]
    virtual auto num_nodes() const -> size_type = 0;
    
    /// Get number of edges.
    [[nodiscard]]
    virtual auto num_edges() const -> size_type = 0;
};

/// Use implementation: e.g using graph_to_use = Graph<MyGraphImplementation>;
template <typename G>
using Graph = std::enable_if_t<std::is_base_of_v<IGraph, G>, G>;

#endif

