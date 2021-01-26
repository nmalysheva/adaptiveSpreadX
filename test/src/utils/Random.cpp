#include <catch.hpp>

#include <utils/Random.hpp>

#include <algorithm>

using utils::random;


TEST_CASE("random_draw_unsigned")
{
    for (auto i = 0u; i < 10u; ++i)
    {
        for (auto j = i; j < 10u; ++j)
        {
            auto const value = random<>(i, j);
            REQUIRE(value == std::clamp(value, i, j));
        }
    }
}

TEST_CASE("random_draw_double")
{
    auto const value = random<double>();
    REQUIRE(value >= 0.0);
    REQUIRE(value <= 1.0);
}

