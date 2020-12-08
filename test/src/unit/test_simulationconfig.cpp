#include <catch.hpp>

#include <configs/SimulationConfig.hpp>
#include <configs/SimulationConfig.cpp>
#include <configs/Segments.cpp>

using Catch::Matchers::Contains;

TEST_CASE("wrong_formats")
{
    auto const empty = "";
    REQUIRE_THROWS_WITH(SimulationConfig{empty}, Contains("0 instead of 3"));

    auto const single_entry = "1";
    REQUIRE_THROWS_WITH(SimulationConfig{single_entry}, Contains("1 instead of 3"));

    auto const too_much = "1 2 3 4";
    REQUIRE_THROWS_WITH(SimulationConfig{too_much}, Contains("4 instead of 3"));

    auto const no_number_1 = "a 0 0";
    REQUIRE_THROWS_AS(SimulationConfig{no_number_1}, std::invalid_argument);

    auto const no_number_2 = "0 b 0";
    REQUIRE_THROWS_AS(SimulationConfig{no_number_2}, std::invalid_argument);
    
    auto const no_number_3 = "0 1 c";
    REQUIRE_THROWS_AS(SimulationConfig{no_number_3}, std::invalid_argument);
}

TEST_CASE("wrong_numbers")
{
    auto const a_greater_b = "1 0 1";
    REQUIRE_THROWS_WITH(SimulationConfig{a_greater_b}, Contains(">"));

    auto const a_equals_b = "0 0 1";
    REQUIRE_THROWS_WITH(SimulationConfig{a_equals_b}, Contains("="));

    auto const negative = "-1 1 1";
    REQUIRE_THROWS_WITH(SimulationConfig{negative}, Contains("negative"));

    auto const steps_too_small = "0 1 0";
    REQUIRE_THROWS_WITH(SimulationConfig{steps_too_small}, Contains("number of simulations"));
}

