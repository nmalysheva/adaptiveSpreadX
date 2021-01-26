#include <catch.hpp>

#include <types/State.hpp>


TEST_CASE("state_check_cast")
{
    auto const a = State{"abc"};
    auto const b = State{"xyz"};
    REQUIRE(static_cast<State::value_type> (a) == "abc");
    REQUIRE(a == a);
    REQUIRE_FALSE(a == b);
    REQUIRE(a < b);
    REQUIRE_FALSE(b < a);
}

