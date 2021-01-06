#include <catch.hpp>

#include <graph/GraphImpl.hpp>

using BasicGraph = Graph<GraphImpl>;


TEST_CASE("empty_graph")
{
    auto const g = BasicGraph{};
    REQUIRE(g.num_nodes() == 0);
    REQUIRE(g.num_edges() == 0);
}


TEST_CASE("add_nodes")
{
    auto g = BasicGraph{};
    for (auto i = 0; i < 10; ++i)
    {
        auto const node = NodeId::create();

        g.add(node);
        REQUIRE(g.num_nodes() == i + 1);
        REQUIRE(g.num_edges() == 0);
        REQUIRE(g.edges_of(node).empty());
        REQUIRE(g.no_edges_of(node).size() == i);
    }
}

