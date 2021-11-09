#include <catch.hpp>

#include <network/IndividualFactory.hpp>

#include <fstream>
#include <string>

using namespace network;


TEST_CASE("individual_factory")
{
    auto const path = std::string{TEST_CONFIG_FOLDER} + "/network/ok.txt";
    auto file = std::ifstream{path};
    auto const cfg = configuration::Configuration{file};
    auto const settings = Settings{cfg};
    auto f = IndividualFactory{settings};
    
    auto const s = State{"A"}; 
    auto const i = f.make(123.0, s);

    REQUIRE(i.modification_time == 123.0);
    REQUIRE(i.state == s);
    REQUIRE(i.new_contact_rate == 1);
    REQUIRE(i.remove_contact_rate == 1);
    REQUIRE(i.transition_rates.size() == 1);

    REQUIRE(f.draw_birth_rates().size() == 1);
}

