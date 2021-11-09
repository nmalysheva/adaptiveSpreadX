#include <catch.hpp>

#include <utils/Parse.hpp>

#include <algorithm>
#include <utility>

using namespace utils::parse;


TEST_CASE("parse_split")
{
    auto const str = std::string{"1"};
    auto const [ok] = to_types<double>(str);
    REQUIRE(ok == 1);

    REQUIRE_THROWS_AS((to_types<double, double>(str)), std::invalid_argument);
}


TEST_CASE("parse_unsigned_ok")
{
    auto const str = std::string{"123"};
    auto const ok = to_type<std::size_t>(str);
    REQUIRE(ok == 123);
}


TEST_CASE("parse_unsigned_fail")
{
    for (auto const& str : {"-123", "a", "1a", "a1", "1.0"})
    {
        try
        {
            std::ignore = to_unsigned(str);
        }
        catch (std::invalid_argument const& e)
        {
            REQUIRE(std::string(e.what()) == NotUnsigned);
        }
        catch (...)
        {
            FAIL();
        }
    }
}


TEST_CASE("parse_double_ok")
{
    auto const str = std::string{"0.123"};
    auto const ok = to_type<double>(str);
    REQUIRE(ok == 0.123);
}


TEST_CASE("parse_double_fail")
{
    for (auto const& str : {"-0.123", ".1", "a"})
    {
        try
        {
            std::ignore = to_double(str);
        }
        catch (std::invalid_argument const& e)
        {
            REQUIRE(std::string(e.what()) == NotDouble);
        }
        catch (...)
        {
            FAIL();
        }
    }
}


TEST_CASE("parse_distribution_ok")
{
    auto const str = std::string{"1"};
    auto const ok = to_type<Distribution>(str);
    REQUIRE(ok.draw() == 1);

    auto const str_2 = std::string{"U(0,1)"};
    auto const ok_2 = to_distribution(str_2).draw();

    REQUIRE(std::clamp(ok_2, 0.0, 1.0) == ok_2);
}


TEST_CASE("parse_distribution_fail")
{
    for (auto const& str : {"-1", "U(0, 1)"})
    {
        try
        {
            std::ignore = to_distribution(str);
        }
        catch (std::invalid_argument const& e)
        {
            auto const a = std::string(e.what()) == NotDistribution;
            auto const b = std::string(e.what()) == NotDouble;
            auto const x = a or b;
            REQUIRE(x);
        }
        catch (...)
        {
            FAIL();
        }
    }
}


TEST_CASE("parse_state")
{
    auto const str = std::string{"X"};
    auto const state = State{"X"};
    auto const ok = to_type<State>(str);
    REQUIRE(ok == state);
}


TEST_CASE("parse_string")
{
    auto const str = std::string{"x"};
    REQUIRE(str == to_type<std::string>(str));
}

