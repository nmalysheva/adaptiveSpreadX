#include <catch.hpp>

#include <Graph.hpp>
#include <graph/GraphImpl.cpp>

using Catch::Matchers::Contains;

TEST_CASE("no_nodes")
{
    REQUIRE_THROWS_WITH((Graph<GraphImpl>{0, 1}), Contains("0 nodes"));
}

TEST_CASE("too_many_edges")
{
    REQUIRE_THROWS_WITH((Graph<GraphImpl>{4, 100}), Contains("too many edges"));
}

