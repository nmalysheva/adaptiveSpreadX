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


struct ForTest final
{
    std::string data{""};

    template <typename I>
    ForTest(I begin, I)
    {
        data = *begin;
    }
};

TEST_CASE("configuration_empty")
{
    auto ss = std::stringstream{};
    auto const config = Configuration{ss};
}

TEST_CASE("configuration_parse_config_ok")
{
    auto ss = std::stringstream{};
    ss << "#Start\n" 
       << marker::HeaderStart << "Test" << marker::HeaderEnd << '\n'
       << "A B C\n"
       << "\n"
       << "#next block\n"
       << marker::HeaderStart << "Data" << marker::HeaderEnd << '\n'
       << "X";
    
    auto const config = Configuration{ss};
    REQUIRE(config.get<ForTest>("Test").data == "A B C");
    REQUIRE(config.get<ForTest>("Data").data == "X");
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

TEST_CASE("configuration_parse_config_section_not_available")
{
    auto ss = std::stringstream{};
    auto const config = Configuration{ss};
    try
    {
        config.get<ForTest>("unknown");
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::UnknownSection);
    }
}

