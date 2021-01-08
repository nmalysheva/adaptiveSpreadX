#include <catch.hpp>

#include <configuration/SpeciesConfiguration.hpp>


TEST_CASE("correct_config")
{
    auto conf = SpeciesConfiguration{};
    REQUIRE_NOTHROW(conf.add("S 0.5 0.1"));
    auto const& data = conf.get();
    REQUIRE(data.size() == 1);
    REQUIRE(std::get<0>(data.front()) == "S");
}


TEST_CASE("incorrect_config")
{
    auto conf = SpeciesConfiguration{};
    REQUIRE_THROWS_AS(conf.add("S 0.5"), std::invalid_argument);
    REQUIRE_THROWS_AS(conf.add("0.1 0.1"), std::invalid_argument);
    REQUIRE_THROWS(conf.add("S A B"));
}

