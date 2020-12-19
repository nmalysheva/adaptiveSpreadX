#include <catch.hpp>

#include <species/Species.hpp>
#include <species/Species.cpp>
#include <species/AttributeFactory.cpp>
#include <types/Propability.cpp>

TEST_CASE("fixed")
{
    auto const config = ConfigurationBlock{"S 1 2", "I 3 4"};

    auto const s = Species{config};

    auto const a = s.create("S");
    REQUIRE(a.state == "S");
    REQUIRE(static_cast<double> (a.new_contact_rate) == 1);
    REQUIRE(static_cast<double> (a.loose_contact_rate) == 2);

    auto const b = s.create("I");
    REQUIRE(b.state == "I");
    REQUIRE(static_cast<double> (b.new_contact_rate) == 3);
    REQUIRE(static_cast<double> (b.loose_contact_rate) == 4);
}

