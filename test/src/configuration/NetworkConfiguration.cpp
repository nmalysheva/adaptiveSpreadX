#include <catch.hpp>

#include <configuration/NetworkConfiguration.hpp>


TEST_CASE("correct_network_config_edges")
{
    auto conf = NetworkConfiguration{};
    REQUIRE(conf.num_edges() == 0);
    REQUIRE_NOTHROW(conf.add("1"));
    REQUIRE(conf.get_nodes().empty());
    REQUIRE(conf.num_edges() == 1);
}


TEST_CASE("incorrect_network_config_edges")
{
    auto conf = NetworkConfiguration{};
    REQUIRE_NOTHROW(conf.add("1"));
    REQUIRE_THROWS_AS(conf.add("2"), std::invalid_argument);
}


TEST_CASE("correct_network_config_nodes")
{
    auto conf = NetworkConfiguration{};
    REQUIRE_NOTHROW(conf.add("S 1"));
    REQUIRE(conf.get_nodes().size() == 1);
    REQUIRE(conf.get_nodes().count("S") == 1);
    REQUIRE(conf.get_nodes().at("S") == 1);
    
    REQUIRE_NOTHROW(conf.add("I 12"));
    REQUIRE(conf.get_nodes().size() == 2);
    REQUIRE(conf.get_nodes().count("I") == 1);
    REQUIRE(conf.get_nodes().at("I") == 12);
}


TEST_CASE("incorrect_network_config_nodes")
{
    auto conf = NetworkConfiguration{};
    REQUIRE_NOTHROW(conf.add("S 1"));
    REQUIRE_THROWS_AS(conf.add("S 2"), std::invalid_argument);
    REQUIRE_THROWS_AS(conf.add("A 1 2"), std::invalid_argument);
}

