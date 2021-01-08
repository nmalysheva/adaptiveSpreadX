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


TEST_CASE("split")
{
    REQUIRE(split("").empty());
    REQUIRE(split("1").size() == 1);
    REQUIRE(split("a b").size() == 2);
}


TEST_CASE("to_distribution")
{
    REQUIRE_NOTHROW(to_distribution("0.5"));
    REQUIRE_NOTHROW(to_distribution("1"));
    REQUIRE_THROWS_AS(to_distribution("."), std::invalid_argument);
    REQUIRE_THROWS_AS(to_distribution(".0"), std::invalid_argument);
    
    REQUIRE_NOTHROW(to_distribution("U(0.1,0.2)"));
    REQUIRE_THROWS_AS(to_distribution("U()"), std::invalid_argument);
    REQUIRE_THROWS_AS(to_distribution("U(,)"), std::invalid_argument);
    REQUIRE_THROWS_AS(to_distribution("U(1,)"), std::invalid_argument);
    REQUIRE_THROWS_AS(to_distribution("U(,1)"), std::invalid_argument);
    REQUIRE_THROWS_AS(to_distribution("U(0,b)"), std::invalid_argument);
    REQUIRE_THROWS_AS(to_distribution("U(a,1)"), std::invalid_argument);
}

TEST_CASE("to_size_t")
{
    REQUIRE_NOTHROW(to_size_t("1"));
    REQUIRE(to_size_t("1") == 1u);

    REQUIRE_THROWS_AS(to_size_t("+1"), std::invalid_argument);
}

