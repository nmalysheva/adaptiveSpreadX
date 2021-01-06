#include <catch.hpp>

#include <utils/String.hpp>


TEST_CASE("trim_empty")
{
    constexpr auto text = "";
    REQUIRE(trim(text) == text);
}

TEST_CASE("trim_leading")
{
    REQUIRE(trim("\ta") == "a");
    REQUIRE(trim("    a") == "a");
    REQUIRE(trim("\t a") == "a");
    REQUIRE(trim("  \t a") == "a");
}

TEST_CASE("trim_trailing")
{
    REQUIRE(trim("a\t") == "a");
    REQUIRE(trim("a    ") == "a");
    REQUIRE(trim("a \t") == "a");
    REQUIRE(trim("a\t ") == "a");
}

TEST_CASE("trim_complete")
{
    REQUIRE(trim("\ta\t") == "a");
}

TEST_CASE("trim_none")
{
    constexpr auto text = "Nothing to trim";
    REQUIRE(trim(text) == text);
}

