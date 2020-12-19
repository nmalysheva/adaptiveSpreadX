#include <catch.hpp>

#include <types/Propability.cpp>
#include <utils/parser.hpp>

#include <cstddef>


using Catch::Matchers::Contains;

TEST_CASE("incorrect_number_of_parameters")
{
    REQUIRE_THROWS_WITH(parse<std::size_t>(""), Contains("0 (should be 1)"));
    REQUIRE_THROWS_WITH(parse<std::size_t>("1 2"), Contains("2 (should be 1)"));
}

TEST_CASE("correct_parse")
{
    auto const [one] = parse<std::size_t>("1");
    REQUIRE(one == 1);

    auto const [a, b, c] = parse<std::string, std::size_t, Propability>("1 2 3.5");
    REQUIRE(a == "1");
    REQUIRE(b == 2);
    REQUIRE(static_cast<double> (c) == 3.5);
}

