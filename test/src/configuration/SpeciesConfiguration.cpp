#include <catch.hpp>

#include <configuration/SpeciesConfiguration.hpp>


TEST_CASE("correct_species_config")
{
    auto conf = SpeciesConfiguration{};
    REQUIRE_NOTHROW(conf.add("S 0.5 0.1"));
    auto const& data = conf.get();
    REQUIRE(data.size() == 1);
    REQUIRE(data.begin()->first == "S");
}


TEST_CASE("incorrect_species_config")
{
    auto conf = SpeciesConfiguration{};
    REQUIRE_THROWS_AS(conf.add("S 0.5"), std::invalid_argument);
    REQUIRE_THROWS_AS(conf.add("0.1 0.1"), std::invalid_argument);
    REQUIRE_THROWS(conf.add("S A B"));

    REQUIRE_NOTHROW(conf.add("S 0 0"));
    REQUIRE_THROWS_AS(conf.add("S 1 1"), std::invalid_argument);
}

