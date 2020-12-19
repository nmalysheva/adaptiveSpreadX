#include <catch.hpp>

#include <species/AttributeFactory.hpp>
#include <species/AttributeFactory.cpp>
#include <types/Propability.cpp>

TEST_CASE("fixed")
{
    auto const [a, b] = AttributeFactory{"1", "2"}.create();
    REQUIRE(static_cast<double> (a) == 1);
    REQUIRE(static_cast<double> (b) == 2);
}

