#include <catch.hpp>

#include <configuration/Configuration.hpp>
#include <configuration/Exception.hpp>
#include <configuration/Helper.hpp>

#include <string>
#include <sstream>
#include <tuple>


using Catch::Matchers::Equals;
using namespace configuration::helper;
using configuration::Configuration;



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
    auto ss = std::stringstream{};
    ss << "#Start\n" 
       << marker::HeaderStart << "Test" << marker::HeaderEnd << '\n'
       << "ABC"
       << "\n"
       << "#next block\n"
       << marker::HeaderStart << "Data" << marker::HeaderEnd << '\n'
       << "X";
    
    auto const config = Configuration{ss};
    REQUIRE(config.get().size() == 2);
}


TEST_CASE("configuration_parse_config_header_twice")
{
    auto ss = std::stringstream{};
    ss << marker::HeaderStart << "Test" << marker::HeaderEnd << '\n'
       << marker::HeaderStart << "Test" << marker::HeaderEnd << '\n';
    try
    {
        Configuration{ss};
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::HeaderAlreadyUsed);
    }
}

