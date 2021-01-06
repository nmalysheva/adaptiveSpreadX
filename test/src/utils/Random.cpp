#include <catch.hpp>

#include <utils/Random.hpp>

#include <algorithm>


TEST_CASE("draw_random_int")
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = i; j < 10; ++j)
        {
            auto const value = random<int>(i, j);
            REQUIRE(value == std::clamp(value, i, j));
        }
    }
}

TEST_CASE("draw_random_double")
{
    auto const value = random<double>();
    REQUIRE(value >= 0.0);
    REQUIRE(value <= 1.0);
}

