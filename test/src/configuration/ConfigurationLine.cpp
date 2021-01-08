#include <catch.hpp>

#include <configuration/ConfigurationLine.hpp>


TEST_CASE("empty_line")
{
    auto const line = ConfigurationLine{""};
    REQUIRE_FALSE(line.is_header());
    REQUIRE(line.content().empty());
}


TEST_CASE("comment")
{
    auto const line = ConfigurationLine{"#I am like an empty line"};
    REQUIRE_FALSE(line.is_header());
    REQUIRE(line.content().empty());
}


TEST_CASE("header")
{
    auto const line = ConfigurationLine{"[Header]"};
    REQUIRE(line.is_header());
    REQUIRE(line.content() == "[Header]");
}

TEST_CASE("data")
{
    auto const line = ConfigurationLine{"any string with data"};
    REQUIRE_FALSE(line.is_header());
    REQUIRE(line.content() == "any string with data");
}

