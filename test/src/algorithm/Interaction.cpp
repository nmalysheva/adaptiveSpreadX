#include <catch.hpp>

#include <algorithm/Interaction.hpp>

using namespace algorithm;


TEST_CASE("interaction_stores_values_correct")
{
    auto const from = State{"A"};
    auto const connected = State{"C"};
    auto const to = State{"B"};
    auto const dist = Distribution{1.0};
    auto const i = Interaction{from, connected, to, dist};
    REQUIRE(i.from() == from);
    REQUIRE(i.to() == to);
    REQUIRE(i.distribution().draw() == 1);
    REQUIRE(i.connected() == connected);
}

TEST_CASE("interaction_compare")
{
    auto const a = Interaction{State{"A"}, State{"B"}, State{"C"}, Distribution{0}};
    auto const b = Interaction{State{"A"}, State{"B"}, State{"D"}, Distribution{1}};
    auto const c = Interaction{State{"B"}, State{"A"}, State{"C"}, Distribution{0}};
    auto const d = Interaction{State{"A"}, State{"A"}, State{"C"}, Distribution{0}};
    auto const e = Interaction{State{"B"}, State{"B"}, State{"C"}, Distribution{0}};

    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
    REQUIRE_FALSE(a == d);
    REQUIRE_FALSE(a == e);
}

