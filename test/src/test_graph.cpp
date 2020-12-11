#include <catch.hpp>

#include <graph/Graph.hpp>
#include <graph/GraphImpl.hpp>
#include <graph/GraphImpl.cpp>

using Catch::Matchers::Contains;

TEST_CASE("too_many_edges")
{
    REQUIRE_THROWS_WITH((Graph<GraphImpl>{4, 100}), Contains("too many edges"));
}

TEST_CASE("correct")
{
    auto const a = GraphImpl{0, 1};
    REQUIRE(a.num_nodes() == 0);
    REQUIRE(a.num_edges() == 0);

    auto const b = GraphImpl{4, 6};
    REQUIRE(b.num_nodes() == 4);
    REQUIRE(b.num_edges() == 6);
}

