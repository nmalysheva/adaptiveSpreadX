#include <catch.hpp>

#include <network/NodeInitilisation.hpp>


TEST_CASE("node_initilisation_init")
{
    auto const a = NodeInitilisation{State{"S"}, 1};
    REQUIRE(a.count() == 1);
    REQUIRE(a.state() == State{"S"});
}

TEST_CASE("compare_node_initilisation")
{
    auto const a = NodeInitilisation{State{"S"}, 1};
    auto const b = NodeInitilisation{State{"S"}, 1};
    auto const c = NodeInitilisation{State{"Z"}, 2};
    auto const d = NodeInitilisation{State{"Z"}, 1};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
    REQUIRE(a < d);
}

