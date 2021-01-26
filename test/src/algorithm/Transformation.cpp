#include <catch.hpp>

#include <algorithm/Transformation.hpp>

using namespace algorithm;


TEST_CASE("tranformation_stores_values_correct")
{
    auto const state = State{"A"};
    auto const dist = Distribution{1.0};
    auto const t = Transformation{state, dist};
    REQUIRE(t.state() == state);
    REQUIRE(t.distribution().draw() == 1);
}

TEST_CASE("transformation_compare")
{
    auto const a = Transformation{State{"A"}, Distribution{0}};
    auto const b = Transformation{State{"A"}, Distribution{1}};
    auto const c = Transformation{State{"B"}, Distribution{0}};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}
