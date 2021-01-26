#include <catch.hpp>

#include <network/ContactNetwork.hpp>

using namespace network;


TEST_CASE("contact_init_empty")
{
    auto settings = Settings{};
    settings.add_factory(State{"S"}, Distribution{0}, Distribution{0});
    auto const cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().empty());
    REQUIRE(cnw.get_edge_creation_rates().empty());
    REQUIRE(cnw.count_specie(State{"S"}) == 0);
}


TEST_CASE("contact_init_nodes_no_edge")
{
    auto settings = Settings{};
    settings.add_factory(State{"S"}, Distribution{0}, Distribution{0});
    settings.add_node(State{"S"}, 2);
    auto const cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().empty());
    REQUIRE(cnw.get_edge_creation_rates().size() == 1);
    REQUIRE(cnw.count_specie(State{"S"}) == 2);
}


TEST_CASE("contact_init_nodes_and_edge")
{
    auto settings = Settings{};
    settings.add_factory(State{"S"}, Distribution{0}, Distribution{0});
    settings.add_node(State{"S"}, 2);
    settings.set_edges(2);

    auto const cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().size() == 1);
    REQUIRE(cnw.get_edge_creation_rates().empty());
    REQUIRE(cnw.count_specie(State{"S"}) == 2);
}


TEST_CASE("contact_change_edges")
{
    auto settings = Settings{};
    settings.add_factory(State{"S"}, Distribution{0}, Distribution{0});
    settings.add_node(State{"S"}, 2);
    settings.set_edges(2);
    auto cnw = ContactNetwork{settings};

    auto const no_edges = cnw.get_edge_deletion_rates();
    REQUIRE(no_edges.size() == 1);
    auto const a = no_edges.front().second.first;
    auto const b = no_edges.front().second.second;

    cnw.delete_edge(b, a);
    REQUIRE(cnw.get_edge_deletion_rates().empty());
    REQUIRE(cnw.get_edge_creation_rates().size() == 1);

    cnw.create_edge(a, b);
    REQUIRE(cnw.get_edge_deletion_rates().size() == 1);
    REQUIRE(cnw.get_edge_creation_rates().empty());
}

TEST_CASE("contact_change_node")
{
    auto settings = Settings{};
    auto const a = State{"A"};
    auto const b = State{"B"};
    settings.add_factory(a, Distribution{0}, Distribution{0});
    settings.add_factory(b, Distribution{0}, Distribution{0});
    auto cnw = ContactNetwork{settings};

    REQUIRE(cnw.count_specie(a) == 0);
    REQUIRE(cnw.count_specie(b) == 0);
    cnw.create(a);
    REQUIRE(cnw.count_specie(a) == 1);
    REQUIRE(cnw.count_specie(b) == 0);

    auto const id_1 = cnw.get_specie(a).front();
    cnw.change(id_1, b);
    REQUIRE(cnw.count_specie(a) == 0);
    REQUIRE(cnw.count_specie(b) == 1);

    auto const id_2 = cnw.get_specie(b).front();
    cnw.remove(id_2);
    REQUIRE(cnw.count_specie(a) == 0);
    REQUIRE(cnw.count_specie(b) == 0);
}


TEST_CASE("contact_get_connections")
{
    auto settings = Settings{};
    auto const a = State{"A"};
    auto const b = State{"B"};
    settings.add_factory(a, Distribution{0}, Distribution{0});
    settings.add_factory(b, Distribution{0}, Distribution{0});
    settings.add_node(a, 2);
    settings.add_node(b, 1);
    settings.set_edges(100);
    auto cnw = ContactNetwork{settings};

    auto connections = cnw.get_connections(a, a);
    REQUIRE(connections.size() == 1);
    connections = cnw.get_connections(a, b);
    REQUIRE(connections.size() == 2);
    connections = cnw.get_connections(b, b);
    REQUIRE(connections.empty());
}

