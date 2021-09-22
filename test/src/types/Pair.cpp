#include <catch.hpp>

#include <types/Pair.hpp>


TEST_CASE("pair_comparison")
{
    auto const a = Pair<double, int>{1.0, 2};
    auto const b = Pair<double, int>{100.0, 0};

    REQUIRE_FALSE(a < b);
}

