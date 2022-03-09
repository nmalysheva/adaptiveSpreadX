#include <catch.hpp>

#include <types/Propability.hpp>


TEST_CASE("create")
{
    REQUIRE_THROWS(Propability{-0.1});
    REQUIRE_THROWS(Propability{1.1});
    REQUIRE_NOTHROW(Propability{0.123});

    auto const p = Propability{0.5};
    REQUIRE(0.5 == static_cast<double> (p));
}

