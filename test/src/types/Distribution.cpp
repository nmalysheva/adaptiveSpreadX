#include <catch.hpp>

#include <types/Distribution.hpp>

#include <algorithm>


TEST_CASE("fixed")
{
    auto const val = Propability{0.1};
    auto dist = Distribution{val};
    REQUIRE(static_cast<Propability::value_type> (dist()) == static_cast<Propability::value_type> (val));
}


TEST_CASE("uniform")
{
    auto const a = 0.0;
    auto const b = 1.0;

    auto dist = Distribution{'U', Propability{a}, Propability{b}};
    auto const val = static_cast<Propability::value_type> (dist());
    REQUIRE(std::clamp(val, a, b) == val);
}


TEST_CASE("unknown_distribution")
{
    auto const v = Propability{0.0};
    REQUIRE_THROWS_AS(Distribution('X', v, v), std::invalid_argument);
}


TEST_CASE("incorrect_parameters")
{
    auto const a = Propability{1.0};
    auto const b = Propability{0.0};
    REQUIRE_THROWS_AS(Distribution('u', a, b), std::invalid_argument);
}

