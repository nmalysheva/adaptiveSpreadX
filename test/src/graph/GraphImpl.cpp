#include <catch.hpp>

#include <graph/GraphImpl.hpp>

#include <iterator>

using BasicGraph = Graph<GraphImpl>;


TEST_CASE("empty_graph")
{
    auto const g = BasicGraph{};
    REQUIRE(g.num_nodes() == 0);
    REQUIRE(g.num_edges() == 0);
}


TEST_CASE("add_and_remove_nodes")
{
    auto g = BasicGraph{};
    constexpr auto total = BasicGraph::size_type{10};
    for (auto i = decltype (total) {0}; i < total; ++i)
    {
        auto const node = NodeId::create();

        g.add(node);
        REQUIRE(g.num_nodes() == i + 1);
        REQUIRE(g.num_edges() == 0);
        REQUIRE(g.edges_of(node).empty());
        REQUIRE(g.no_edges_of(node).size() == i);
    }

    for (auto i = decltype (total) {0}; i < total; ++i)
    {
        auto const node = NodeId::refer(i);
        g.remove(node);
        REQUIRE(g.num_nodes() == (total - i - 1));
        REQUIRE(g.num_edges() == 0);
    }
}

TEST_CASE("add_and_remove_edges")
{
    auto g = BasicGraph{};
    auto const a = NodeId::create();
    auto const b = NodeId::create();
    auto const c = NodeId::create();
    g.add(a);
    g.add(b);
    g.add(c);

    REQUIRE(g.num_nodes() == 3);
    REQUIRE(g.num_edges() == 0);

    g.connect(a, b);
    REQUIRE(g.num_edges() == 1);
    REQUIRE(g.edges_of(a).size() == 1);
    REQUIRE(g.no_edges_of(a).size() == 1);
    REQUIRE(g.edges_of(a).count(b) == 1);
    REQUIRE(g.edges_of(b).empty());
    REQUIRE(g.edges_of(c).empty());
    
    g.connect(a, c);
    REQUIRE(g.num_edges() == 2);
    REQUIRE(g.edges_of(a).size() == 2);
    REQUIRE(g.no_edges_of(a).empty());
    REQUIRE(g.edges_of(a).count(c) == 1);
    REQUIRE(g.edges_of(b).empty());
    REQUIRE(g.edges_of(c).empty());

    g.connect(b, c);
    REQUIRE(g.num_edges() == 3);
    REQUIRE(g.edges_of(b).size() == 1);
    REQUIRE(g.no_edges_of(b).size() == 1);
    REQUIRE(g.edges_of(c).empty());

    g.disconnect(b,c);
    REQUIRE(g.num_edges() == 2);
    REQUIRE(g.edges_of(b).empty());
}

