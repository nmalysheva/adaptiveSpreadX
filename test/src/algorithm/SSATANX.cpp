#include <catch.hpp>

#include <algorithm/SSATANX.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <utils/Json.hpp>

#include <fstream>

namespace
{
auto make_config(std::string const& name)
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/algorithm/ssatan-x/" + name};
    return configuration::Configuration{fs};
}
} // namespace


TEST_CASE("SSATANX_nothing")
{
    auto const config = make_config("nothing.txt");
    auto network = network::ContactNetwork{config};
    auto ssatanx = algorithm::make_algorithm(config, network);
    auto json = utils::json::Block{};
    ssatanx->run(json);

    SUCCEED();
}


TEST_CASE("SSATANX_full")
{
    auto const config = make_config("full.txt");
    auto network = network::ContactNetwork{config};
    auto ssatanx = algorithm::SSATANX{config, network};
    auto json = utils::json::Block{};
    ssatanx.run(json);

    SUCCEED();
}


TEST_CASE("SSATANX_only_births")
{
    auto const config = make_config("only_births.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_deaths().empty());

    auto ssatanx = algorithm::SSATANX{config, network};
    auto json = utils::json::Block{};
    ssatanx.run(json);
    
    auto const& states = network.get_state_counts();
    REQUIRE(states.size() == 1);
    REQUIRE(states.begin()->second > 0);
}


TEST_CASE("SSATANX_only_deaths")
{
    auto const config = make_config("only_deaths.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_deaths().size() == 1);

    auto ssatanx = algorithm::SSATANX{config, network};
    auto json = utils::json::Block{};
    ssatanx.run(json);
    REQUIRE(network.get_deaths().empty());
}


TEST_CASE("SSATANX_only_transitions")
{
    auto const config = make_config("only_transitions.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_transitions().size() == 1);

    auto ssatanx = algorithm::SSATANX{config, network};
    auto json = utils::json::Block{};
    ssatanx.run(json);
    REQUIRE(network.get_transitions().empty());
}


TEST_CASE("SSATANX_only_interactions")
{
    auto const config = make_config("only_interactions.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_interactions().size() == 1);

    auto ssatanx = algorithm::SSATANX{config, network};
    auto json = utils::json::Block{};
    ssatanx.run(json);
    REQUIRE(network.get_interactions().empty());
}

