#include <catch.hpp>

#include <algorithm/SSA.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <settings/Settings.hpp>
#include <utils/Json.hpp>

#include <fstream>


TEST_CASE("SSA_nothing")
{
    auto file = std::ifstream{"../../test/configs/nothing.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);

    SUCCEED();
}


TEST_CASE("SSA_only_births")
{
    auto file = std::ifstream{"../../test/configs/only_births.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    REQUIRE(network.get_deaths().empty());

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_deaths().size() == 1);
}


TEST_CASE("SSA_only_deaths")
{
    auto file = std::ifstream{"../../test/configs/only_deaths.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    REQUIRE(network.get_deaths().size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_deaths().empty());
}


TEST_CASE("SSA_only_new_edges")
{
    auto file = std::ifstream{"../../test/configs/only_new_edges.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    REQUIRE(network.get_edge_creation_rates().size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_edge_creation_rates().empty());
}


TEST_CASE("SSA_only_delete_edges")
{
    auto file = std::ifstream{"../../test/configs/only_delete_edges.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    REQUIRE(network.get_edge_deletion_rates().size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_edge_deletion_rates().empty());
}


TEST_CASE("SSA_only_transitions")
{
    auto file = std::ifstream{"../../test/configs/only_transitions.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    REQUIRE(network.get_transitions().size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_transitions().empty());
}



TEST_CASE("SSA_only_interactions")
{
    auto file = std::ifstream{"../../test/configs/only_interactions.txt"};
    auto const config = configuration::Configuration{file};
    auto const settings = settings::Settings{config.get()};
    auto network = network::ContactNetwork{settings.network()};
    REQUIRE(network.get_interactions().size() == 1);

    auto ssa = algorithm::SSA{settings.algorithm(), network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_interactions().empty());
}

