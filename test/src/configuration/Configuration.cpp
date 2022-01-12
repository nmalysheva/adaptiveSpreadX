#include <catch.hpp>

#include <configuration/Configuration.hpp>
#include <configuration/Exception.hpp>

#include <fstream>
#include <string>
#include <sstream>


using Catch::Matchers::Equals;
using configuration::Configuration;


auto const PATH = std::string{TEST_CONFIG_FOLDER} + "/configuration/";


TEST_CASE("configuration_empty")
{
    auto ss = std::stringstream{};
    try
    {
        auto const config = Configuration{ss};
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string(e.what()) == configuration::error::NoSections);
    }
}


TEST_CASE("configuration_parse_config_ok")
{
    auto fs = std::ifstream{PATH + "simple.txt"};
    auto const config = Configuration{fs};
}


TEST_CASE("configuration_parse_config_header_twice")
{
    auto fs = std::ifstream{PATH + "header_twice.txt"};
    try
    {
        Configuration{fs};
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::HeaderAlreadyUsed);
    }
}


TEST_CASE("configuration_get")
{
    auto fs = std::ifstream{PATH + "simple.txt"};
    auto const config = Configuration{fs};

    REQUIRE(config.get_unused() not_eq std::nullopt);

    auto const first = config.get("FIRST")->get();
    REQUIRE(first.size() == 1);
    REQUIRE(first.front() == "ABC");
    REQUIRE(config.get_unused() not_eq std::nullopt);

    auto const second = config.get("SECOND")->get();
    REQUIRE(second.size() == 3);
    REQUIRE(config.get_unused() not_eq std::nullopt);

    auto const empty = config.get("EMPTY")->get();
    REQUIRE(empty.empty());

    REQUIRE(config.get_unused() == std::nullopt);
    REQUIRE(config.get("NA") == std::nullopt);
}


TEST_CASE("configuration_json")
{
    auto fs = std::ifstream{PATH + "json.txt"};
    auto const config = Configuration{fs};
    auto const Result = std::string{"{\"X\":[\"A\",\"B\",\"C\"]}"};
    REQUIRE(config.to_json() == Result);
}

