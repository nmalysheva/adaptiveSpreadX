#include <catch.hpp>

#include <network/NodeInitilisation.hpp>


TEST_CASE("compare_node_initilisation")
{
    auto const a = NodeInitilisation{"S", 1};
    auto const b = NodeInitilisation{"S", 2};
    auto const c = NodeInitilisation{"I", 0};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

