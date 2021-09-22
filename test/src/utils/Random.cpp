#include <catch.hpp>

#include <utils/Random.hpp>

#include <algorithm>


TEST_CASE("random_draw_double")
{
    auto const drawn = utils::random_double(1.0);
    auto const value = std::clamp(drawn, 0.0, 1.0);
    REQUIRE(value == drawn);
    REQUIRE(value not_eq 0.0);
}

