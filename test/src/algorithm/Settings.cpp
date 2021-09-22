#include <catch.hpp>

#include <algorithm/Settings.hpp>

using namespace algorithm;


TEST_CASE("settings_time_ok")
{
    auto s = Settings{};
    const auto time = 0.1;
    s.set_time(time);
    REQUIRE(s.time() == time);
}


TEST_CASE("settings_time_twice")
{
    auto s = Settings{};
    s.set_time(0.1);
    REQUIRE_THROWS(s.set_time(0.2));
}


TEST_CASE("settings_output_set_ok")
{
    auto s = Settings{};
    const auto output = 0;
    REQUIRE_NOTHROW(s.set_output_step(output));
    REQUIRE(s.output_step() == std::pow(10, output));
}


TEST_CASE("settings_output_twice")
{
    auto s = Settings{};
    REQUIRE_NOTHROW(s.set_output_step(0));
    REQUIRE_THROWS(s.set_output_step(1));
}


TEST_CASE("settings_output_noinit")
{
    auto s = Settings{};
    REQUIRE(s.output_step() == 0);
}

