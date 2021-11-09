#include <catch.hpp>

#include <algorithm/SSA.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <utils/Json.hpp>

#include <fstream>


auto make_config(std::string const& name)
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/algorithm/" + name};
    return configuration::Configuration{fs};
}


TEST_CASE("SSA_nothing")
{
    auto const config = make_config("nothing.txt");
    auto network = network::ContactNetwork{config};
    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);

    SUCCEED();
}


TEST_CASE("SSA_only_births")
{
    auto const config = make_config("only_births.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_deaths().empty());

    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_deaths().size() == 1);
}


TEST_CASE("SSA_only_deaths")
{
    auto const config = make_config("only_deaths.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_deaths().size() == 1);

    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_deaths().empty());
}


TEST_CASE("SSA_only_new_edges")
{
    auto const config = make_config("only_new_edges.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_edge_creation_rates().size() == 1);

    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_edge_creation_rates().empty());
}


TEST_CASE("SSA_only_remove_edges")
{
    auto const config = make_config("only_delete_edges.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_edge_deletion_rates().size() == 1);

    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_edge_deletion_rates().empty());
}


TEST_CASE("SSA_only_transitions")
{
    auto const config = make_config("only_transitions.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_transitions().size() == 1);

    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_transitions().empty());
}


TEST_CASE("SSA_only_interactions")
{
    auto const config = make_config("only_interactions.txt");
    auto network = network::ContactNetwork{config};
    REQUIRE(network.get_interactions().size() == 1);

    auto ssa = algorithm::SSA{config, network};
    auto json = utils::json::Block{};
    ssa.run(json);
    REQUIRE(network.get_interactions().empty());
}

