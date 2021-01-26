#include <catch.hpp>

#include <algorithm/Transition.hpp>

using namespace algorithm;


TEST_CASE("transition_stores_values_correct")
{
    auto const from = State{"A"};
    auto const to = State{"B"};
    auto const dist = Distribution{1.0};
    auto const t = Transition{from, to, dist};
    REQUIRE(t.from() == from);
    REQUIRE(t.to() == to);
    REQUIRE(t.distribution().draw() == 1);
}

TEST_CASE("transition_compare")
{
    auto const a = Transition{State{"A"}, State{"B"}, Distribution{0}};
    auto const b = Transition{State{"A"}, State{"B"}, Distribution{1}};
    auto const c = Transition{State{"A"}, State{"C"}, Distribution{0}};
    auto const d = Transition{State{"C"}, State{"B"}, Distribution{0}};
    auto const e = Transition{State{"C"}, State{"C"}, Distribution{0}};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
    REQUIRE_FALSE(a == d);
    REQUIRE_FALSE(a == e);
}
