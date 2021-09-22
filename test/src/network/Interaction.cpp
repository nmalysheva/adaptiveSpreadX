#include <catch.hpp>

#include <network/Interaction.hpp>

using network::Interaction;


TEST_CASE("interaction_comparison")
{
    const auto sa = State{"a"};
    const auto sb = State{"b"};

    auto const a = Interaction{sa, sb, sa, Distribution::Ignore};
    auto const b_a = Interaction{sb, sa, sa, Distribution::Ignore};
    REQUIRE(a < b_a);
    REQUIRE_FALSE(b_a < a);

    auto const b_b = Interaction{sb, sa, sb, Distribution::Ignore};
    REQUIRE(a < b_b);
    REQUIRE_FALSE(b_b < a);

    auto const c = Interaction{sa, sb, sa,  Distribution::Ignore};
    REQUIRE(c < b_a);
    REQUIRE(c < b_b);
}

