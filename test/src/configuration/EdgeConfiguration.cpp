#include <catch.hpp>

#include <configuration/EdgeConfiguration.hpp>

#include <limits>


TEST_CASE("correct_network_config_edges")
{
    auto conf = EdgeConfiguration{};
    REQUIRE(conf.get() == std::numeric_limits<std::size_t>::max());
    REQUIRE_NOTHROW(conf.add("1"));
    REQUIRE(conf.get() == 1);
}


TEST_CASE("incorrect_network_config_edges_twice")
{
    auto conf = EdgeConfiguration{};
    REQUIRE_NOTHROW(conf.add("1"));
    REQUIRE_THROWS_AS(conf.add("2"), std::invalid_argument);
}

TEST_CASE("incorrect_network_config_edges_format")
{
    auto conf = EdgeConfiguration{};
    REQUIRE_THROWS_AS(conf.add("1 2"), std::invalid_argument);
}

