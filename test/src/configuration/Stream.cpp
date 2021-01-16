#include <catch.hpp>

#include <configuration/Exception.hpp>
#include <configuration/Stream.hpp>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>


using Catch::Matchers::Equals;


TEST_CASE("stream_read")
{
    auto ss = std::stringstream{};
    ss << "#x\nAA\n BB\n\n CC ";
    auto i = 0u;
    auto stream = configuration::Stream{ss};

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
        auto const name = std::string{std::tmpnam(nullptr)};
        auto fs = std::ifstream{name};
        configuration::Stream{fs};
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::NotReadable);
    }
}

