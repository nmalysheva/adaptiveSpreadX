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

    auto const t2 = network.get_transitions().back();
    network.change(124, t2.from, t2.to);
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


TEST_CASE("contactnetwork_adaption_edges")
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/adaption_only_edges_ok.txt"};
    auto const config = configuration::Configuration{fs};
    auto const settings = Settings{config};
    auto network = ContactNetwork{settings};
    
    REQUIRE(network.get_edge_deletion_rates().size() == 1);
    REQUIRE(network.get_edge_creation_rates().empty());

    auto const t = network.get_transitions().front();
    network.change(123, t.from, t.to);

    REQUIRE(network.get_edge_deletion_rates().empty());
    REQUIRE(network.get_edge_creation_rates().size() == 1);
}


TEST_CASE("contactnetwork_adaption_nodes")
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/adaption_only_nodes_ok.txt"};
    auto const config = configuration::Configuration{fs};
    auto const settings = Settings{config};
    auto network = ContactNetwork{settings};

    auto const& counts = network.get_state_counts();
    auto a = State{"A"};
    auto b = State{"B"};
    auto c = State{"C"};
    auto d = State{"D"};
    auto e = State{"E"};
    auto f = State{"F"};
    auto g = State{"G"};
    REQUIRE(counts.at(a) == 1);
    REQUIRE(counts.at(b) == 1);
    REQUIRE(counts.at(c) == 2);
    REQUIRE(counts.at(d) == 0);
    REQUIRE(counts.at(e) == 0);
    REQUIRE(counts.at(f) == 1);
    REQUIRE(counts.at(g) == 0);

    auto const t = network.get_transitions().front();
    network.change(123, t.from, t.to);
    
    REQUIRE(counts.at(a) == 0);
    REQUIRE(counts.at(b) == 1);
    REQUIRE(counts.at(c) == 1);
    REQUIRE(counts.at(d) == 1);
    REQUIRE(counts.at(e) == 1);
    REQUIRE(counts.at(f) == 1);
    REQUIRE(counts.at(g) == 0);
    
    auto const t2 = network.get_transitions().back();
    network.change(124, t2.from, t2.to);
    
    REQUIRE(counts.at(a) == 0);
    REQUIRE(counts.at(b) == 1);
    REQUIRE(counts.at(c) == 1);
    REQUIRE(counts.at(d) == 1);
    REQUIRE(counts.at(e) == 1);
    REQUIRE(counts.at(f) == 0);
    REQUIRE(counts.at(g) == 1);
}


TEST_CASE("contactnetwork_adaption_skip_lonely")
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/adaption_skip_lonely.txt"};
    auto const config = configuration::Configuration{fs};
    auto const settings = Settings{config};
    auto network = ContactNetwork{settings};

    auto const& counts = network.get_state_counts();
    auto a = State{"A"};
    auto b = State{"B"};
    REQUIRE(counts.at(a) == 1);
    REQUIRE(counts.at(b) == 1);

    auto const t = network.get_transitions().front();
    network.change(123, t.from, t.to);
    
    REQUIRE(counts.at(a) == 0);
    REQUIRE(counts.at(b) == 2);
}


TEST_CASE("contactnwetork_adaption_no_change_to_same")
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/adaption_no_change_to_same.txt"};
    auto const config = configuration::Configuration{fs};
    auto const settings = Settings{config};
    auto network = ContactNetwork{settings};

    auto const& counts = network.get_state_counts();
    auto a = State{"A"};
    auto b = State{"B"};
    auto c = State{"C"};
    REQUIRE(counts.at(a) == 1);
    REQUIRE(counts.at(b) == 1);
    REQUIRE(counts.at(c) == 0);

    auto const t = network.get_transitions().front();
    network.change(123, t.from, t.to);
    
    REQUIRE(counts.at(a) == 0);
    REQUIRE(counts.at(b) == 1);
    REQUIRE(counts.at(c) == 1);
}

