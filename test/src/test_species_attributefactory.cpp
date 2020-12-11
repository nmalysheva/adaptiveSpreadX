#include <catch.hpp>

#include <species/AttributeFactory.hpp>
#include <species/AttributeFactory.cpp>

TEST_CASE("fixed")
{
    auto const [a, b] = AttributeFactory{"1", "2"}.create();
    REQUIRE(a == 1);
    REQUIRE(b == 2);
}

