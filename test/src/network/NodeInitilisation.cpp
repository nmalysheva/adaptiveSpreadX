#include <catch.hpp>

#include <network/NodeInitilisation.hpp>



#include <string>
#include <sstream>


TEST_CASE("compare_node_initilisation")
{
    auto const a = NodeInitilisation{"S 1"};
    auto const b = NodeInitilisation{"S 1"};
    auto const c = NodeInitilisation{"Z 2"};
    auto const d = NodeInitilisation{"Z 1"};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
    REQUIRE(a < d);
}

