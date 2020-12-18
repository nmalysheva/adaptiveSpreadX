#ifndef GRAPH_GRAPHIMPL_HPP_
#define GRAPH_GRAPHIMPL_HPP_

#include <graph/Graph.hpp>

#include <string>
#include <unordered_set>
#include <unordered_map>


/*!
 * \brief Basic implementation of IGraph.
 *
 * This is a very basic, not optimised, implementation of a directed graph.
 * For faster and more efficient usage a proper graph library should be used.
 */
class GraphImpl final : public IGraph
{
  public:
    GraphImpl() = delete;

    /*!
     * \brief Create a directed graph.
     *
     * The graph will contain num_nodes nodes.
     * num_edges edges are randomly created.
     *
     * num_edges must not exceed "num_nodes over 2".
     */
    GraphImpl(size_type const num_nodes, size_type const num_edges);

    /// Get number of nodes.
    auto num_nodes() const -> size_type override;

    /// Get number of edges.
    auto num_edges() const -> size_type override;
    
    auto print_edges() const -> void override;
    
    auto edges_of(size_type const node) const -> node_collection_type const& override;
    auto no_edges_of(size_type const node) const -> node_collection_type const& override;
    
    auto connect(size_type const from, size_type const to) -> void override;
    auto disconnect(size_type const from, size_type const to) -> void override;

  private:
    /// All connections
    std::unordered_map<size_type, std::unordered_set<size_type>> m_edges;

    /// All unused connections.
    std::unordered_map<size_type, std::unordered_set<size_type>> m_loose;

    /// Throw exception with text "msg".
    [[noreturn]]
    auto raise(std::string&& msg) -> void;
};

#endif

