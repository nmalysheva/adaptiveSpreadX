#include <catch.hpp>

#include <algorithm/SSA.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <settings/Settings.hpp>
#include <utils/Json.hpp>

#include <fstream>


TEST_CASE("SSA_nothing_to_do")
{
    auto file = std::ifstream{"../../test/configs/nothing.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto ssa = algorithm::SSA{settings.algorithm(), network};
    REQUIRE_FALSE(ssa.execute());
}


TEST_CASE("SSA_only_births")
{
    auto file = std::ifstream{"../../test/configs/only_births.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto const state = State{"S"};
    REQUIRE(network.get_specie(state).empty());

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE_FALSE(network.get_specie(state).empty());
}


TEST_CASE("SSA_only_deaths")
{
    auto file = std::ifstream{"../../test/configs/only_deaths.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto const state = State{"S"};
    REQUIRE(network.get_specie(state).size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_specie(state).empty());
}


TEST_CASE("SSA_only_new_edges")
{
    auto file = std::ifstream{"../../test/configs/only_new_edges.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto const state = State{"S"};
    REQUIRE(network.get_specie(state).size() == 2);
    REQUIRE(network.get_connections(state, state).empty());

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_specie(state).size() == 2);
    REQUIRE(network.get_connections(state, state).size() == 1);
}


TEST_CASE("SSA_only_delete_edges")
{
    auto file = std::ifstream{"../../test/configs/only_delete_edges.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto const state = State{"S"};
    REQUIRE(network.get_specie(state).size() == 2);
    REQUIRE(network.get_connections(state, state).size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_specie(state).size() == 2);
    REQUIRE(network.get_connections(state, state).empty());
}


TEST_CASE("SSA_only_transitions")
{
    auto file = std::ifstream{"../../test/configs/only_transitions.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto const s = State{"S"};
    auto const i = State{"I"};
    REQUIRE(network.get_specie(s).size() == 1);
    REQUIRE(network.get_specie(i).empty());

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_specie(s).empty());
    REQUIRE(network.get_specie(i).size() == 1);
}


TEST_CASE("SSA_only_interactions")
{
    auto file = std::ifstream{"../../test/configs/only_interactions.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    auto const s = State{"S"};
    auto const i = State{"I"};
    REQUIRE(network.get_specie(s).size() == 1);
    REQUIRE(network.get_specie(i).size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_specie(s).empty());
    REQUIRE(network.get_specie(i).size() == 2);
}

