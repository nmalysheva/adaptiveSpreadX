#include <catch.hpp>

#include <types/Tripel.hpp>


TEST_CASE("tripel_comparison")
{
    auto const a = Tripel<double, int, int>{0.0, 1, 2};
    auto const b = Tripel<double, int, int>{100.0, 0, 2};

    REQUIRE_FALSE(a < b);
    REQUIRE(b < a);

    auto const c = Tripel<double, int, int>{1.0, 1, 3};
    REQUIRE(a < c);
    REQUIRE_FALSE(c < a);
}

