#include <catch.hpp>

#include <network/ContactNetwork.hpp>

#include <fstream>


using namespace network;


auto make_network()
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/ok.txt"};
    auto const config = configuration::Configuration{fs};
    auto const settings = Settings{config};
    return ContactNetwork{settings};
}


TEST_CASE("contactnetwork_init_no_edges")
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/ok_no_edges.txt"};
    auto const config = configuration::Configuration{fs};
    auto const settings = Settings{config};
    auto network = ContactNetwork{settings};
    
    REQUIRE(network.get_transitions().size() == 2);
    REQUIRE(network.get_edge_creation_rates().size() == 1);
    REQUIRE(network.get_edge_deletion_rates().empty());
}

TEST_CASE("contactnetwork_init")
{
    auto network = make_network();

    REQUIRE(network.get_transitions().size() == 2);
    REQUIRE(network.get_edge_creation_rates().empty());
    REQUIRE(network.get_edge_deletion_rates().size() == 1);
    
    auto edge = network.get_edge_deletion_rates().front();
    REQUIRE(edge.rate == 0.1);

    REQUIRE(network.get_births().size() == 1);
    REQUIRE(network.get_deaths().size() == 1);
    REQUIRE(network.get_interactions().size() == 1);

    auto const state_count = network.get_state_counts();
    REQUIRE(state_count.size() == 2);
    auto it = state_count.begin();
    REQUIRE(it->second == 1);
    ++it;
    REQUIRE(it->second == 1);

    REQUIRE(network.get_max_interaction_rates().size() == 2);
}


TEST_CASE("contactnetwork_delete_node")
{
    auto network = make_network();
    auto const d = network.get_deaths().front();
    network.remove(d.identifier);
    REQUIRE(network.get_transitions().size() == 1);
    REQUIRE(network.get_edge_deletion_rates().empty());
    REQUIRE(network.get_edge_creation_rates().empty());
    REQUIRE(network.get_interactions().empty());
}


TEST_CASE("contactnetwork_create_node")
{
    auto network = make_network();
    auto const b = network.get_births().front();
    network.create(123, b.identifier);
    
    REQUIRE(network.get_transitions().size() == 3);
    REQUIRE(network.get_edge_deletion_rates().size() == 1);
    REQUIRE(network.get_edge_creation_rates().size() == 2);
}


TEST_CASE("contactnetwork_change_node")
{
    auto network = make_network();
    auto const t = network.get_transitions().front();
    network.change(123, t.from, t.to);

    REQUIRE(network.get_edge_deletion_rates().size() == 1);
}


TEST_CASE("contactnetwork_modify_edges")
{
    auto network = make_network();

    REQUIRE(network.get_interactions().size() == 1);
    REQUIRE(network.get_edge_creation_rates().empty());
    auto const e = network.get_edge_deletion_rates().front();
    network.delete_edge(e.from, e.to);
    auto const undo = network.get_last_edge_undo();

    auto const from_ok = (undo.from == e.from) or (undo.from == e.to);
    auto const to_ok = (undo.to == e.from) or (undo.to == e.to);
    REQUIRE(from_ok);
    REQUIRE(to_ok);
    auto const diff = not (undo.from == undo.to);
    REQUIRE(diff);

    REQUIRE(network.get_edge_deletion_rates().empty());
    REQUIRE(network.get_edge_creation_rates().size() == 1);
    REQUIRE(network.get_interactions().empty());
}

