#include <catch.hpp>

#include <configuration/Helper.hpp>
#include <configuration/Exception.hpp>

#include <string>
#include <sstream>
#include <tuple>


using Catch::Matchers::Equals;
using namespace configuration::helper;


TEST_CASE("helper_classify")
{
    REQUIRE(classify("") == Category::eos);
    REQUIRE(classify(std::string{marker::HeaderStart, marker::HeaderEnd}) == Category::header);
    REQUIRE(classify(std::string{marker::Comment}) == Category::comment);
    REQUIRE(classify("abc") == Category::data);
    REQUIRE_FALSE(classify(std::string{marker::HeaderStart}) == Category::header);
}


TEST_CASE("helper_find_first_header_none")
{
    auto ss = std::stringstream{};
    ss << "#abc";
    auto stream = configuration::Stream{ss};
    REQUIRE_FALSE(find_first_header(stream));
}


TEST_CASE("helper_find_first_header_available")
{
    auto ss = std::stringstream{};
    ss << marker::HeaderStart << "abc" << marker::HeaderEnd;
    auto stream = configuration::Stream{ss};
    REQUIRE(find_first_header(stream));
}


TEST_CASE("helper_find_first_header_data")
{
    auto ss = std::stringstream{};
    ss << "xyz";
    auto stream = configuration::Stream{ss};
    try
    {
        std::ignore = find_first_header(stream);
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::NoHeader);
    }
}

TEST_CASE("helper_unheader")
{
    auto const in_ok = std::string{marker::HeaderStart, 'a', 'b', 'c', marker::HeaderEnd};
    REQUIRE(unheader(in_ok) == "abc");

    auto const in_fail = std::string{marker::HeaderStart, marker::HeaderEnd};
    try
    {
        std::ignore = unheader(in_fail);
        FAIL();
    }
    catch (configuration::Exception const& e)
    {
        REQUIRE(std::string{e.what()} == configuration::error::EmptyHeader);
    }
}

