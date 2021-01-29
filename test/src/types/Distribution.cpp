#include <catch.hpp>

#include <types/Distribution.hpp>

#include <algorithm>


TEST_CASE("distribution_constant_value")
{
    auto const val = 0.1;
    auto const dist = Distribution{val};
    REQUIRE(dist.draw() == val);
}


TEST_CASE("distribution_n_times")
{
    auto const val = 1.0;
    auto const dist = Distribution{val};
    REQUIRE(dist.draw(5) == 5.0);
}


TEST_CASE("distribution_uniform_value")
{
    auto const a = 0.0;
    auto const b = 1.0;
    auto dist = Distribution{'U', a, b};
    auto const val = dist.draw();
    REQUIRE(std::clamp(val, a, b) == val);
}


TEST_CASE("distribution_unknown_distribution")
{
    auto const v = 0.0;
    REQUIRE_THROWS_AS(Distribution('X', v, v), std::invalid_argument);
}


TEST_CASE("distribution_incorrect_parameters")
{
    auto const a = 1.0;
    auto const b = 0.0;
    REQUIRE_THROWS_AS(Distribution('U', a, b), std::invalid_argument);
}

TEST_CASE("distribution_not_0_1_fixed")
{
    try
    {
        std::ignore = Distribution{5.0};
        FAIL();
    }
    catch (std::out_of_range const& e)
    {
        REQUIRE(std::string{e.what()} == Distribution::OutOfRange);
    }
}

TEST_CASE("distribution_not_0_1_uniform")
{
    try
    {
        std::ignore = Distribution{'U', 0.0, 1.1};
        FAIL();
    }
    catch (std::out_of_range const& e)
    {
        REQUIRE(std::string{e.what()} == Distribution::OutOfRange);
    }
}
