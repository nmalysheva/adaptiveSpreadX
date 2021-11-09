#include <catch.hpp>

#include <configuration/Exception.hpp>
#include <configuration/Stream.hpp>

#include <fstream>
#include <string>


using Catch::Matchers::Equals;

auto const PATH = std::string{TEST_CONFIG_FOLDER} + "/configuration/";


TEST_CASE("stream_read")
{
    auto fs = std::ifstream{PATH + "stream_test.txt"};
    auto i = 0u;
    auto stream = configuration::Stream{fs};

    while (stream.has_next_line())
    {
        ++i;
        REQUIRE_FALSE(stream.get_line().empty());
        REQUIRE(stream.get_line().size() == 2);
    }

    REQUIRE(i == 4);
    REQUIRE(stream.get_line().empty());
}


TEST_CASE("stream_not_readable")
{
    try
    {
        auto fs = std::ifstream{PATH + "not_there"};
        configuration::Stream{fs};
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::NotReadable);
    }
}

