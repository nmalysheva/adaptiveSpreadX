#include <catch.hpp>

#include <types/Propability.hpp>


TEST_CASE("create_ok")
{
    REQUIRE_NOTHROW(Propability{0.0});
    REQUIRE_NOTHROW(Propability{0.5});
    REQUIRE_NOTHROW(Propability{1.0});
}

TEST_CASE("create_failed")
{
    REQUIRE_THROWS_AS(Propability{-1.0}, std::out_of_range);
    REQUIRE_THROWS_AS(Propability{1.1}, std::out_of_range);
}


TEST_CASE("convert")
{
    auto const value = Propability::value_type{0.5};
    auto const prop = Propability{value};
    REQUIRE(static_cast<Propability::value_type> (prop) == value);
}

