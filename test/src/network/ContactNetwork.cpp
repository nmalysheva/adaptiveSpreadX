#include <catch.hpp>

#include <network/ContactNetwork.hpp>

using namespace network;


TEST_CASE("contact_init_empty")
{
    auto settings = Settings{};
    auto const state = State{"s"};
    settings.add_state(state);
    settings.add_edge_creation_distribution(state, Distribution::Ignore);
    settings.add_edge_removal_distribution(state, Distribution::Ignore);

    auto const cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().empty());
    REQUIRE(cnw.get_edge_creation_rates().empty());
    REQUIRE(cnw.get_deaths().empty());
}


TEST_CASE("contact_init_nodes_no_edge")
{
    auto settings = Settings{};
    auto const state = State{"s"};
    settings.add_state(state);
    settings.add_edge_creation_distribution(state, Distribution{0.1});
    settings.add_edge_removal_distribution(state, Distribution::Ignore);
    settings.add_node(state, 2);

    auto const cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().empty());
    REQUIRE(cnw.get_edge_creation_rates().size() == 1);
}


TEST_CASE("contact_init_nodes_and_edge")
{
    auto settings = Settings{};
    auto const state = State{"s"};
    settings.add_state(state);
    settings.add_edge_creation_distribution(state, Distribution::Ignore);
    settings.add_edge_removal_distribution(state, Distribution{0.1});
    settings.add_node(state, 2);
    settings.set_edges(5);

    auto const cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().size() == 1);
    REQUIRE(cnw.get_edge_creation_rates().empty());
}


TEST_CASE("contact_create_delete_edge")
{
    auto settings = Settings{};
    auto const state = State{"s"};
    settings.add_state(state);
    settings.add_edge_creation_distribution(state, Distribution{0.1});
    settings.add_edge_removal_distribution(state, Distribution{0.1});
    settings.add_node(state, 2);

    auto cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_edge_deletion_rates().empty());

    auto rates = cnw.get_edge_creation_rates();
    REQUIRE(rates.size() == 1);

    cnw.create_edge(rates.front().from, rates.front().to);
    REQUIRE(cnw.get_edge_creation_rates().empty());
   
    rates.clear(); 
    rates = cnw.get_edge_deletion_rates();
    REQUIRE(rates.size() == 1);
    
    cnw.delete_edge(rates.front().to, rates.front().from);
    REQUIRE(cnw.get_edge_deletion_rates().empty());
    REQUIRE(cnw.get_edge_creation_rates().size() == 1);
}


TEST_CASE("contact_create_delete_node")
{
    auto settings = Settings{};
    auto const state = State{"s"};
    settings.add_state(state);
    settings.add_edge_creation_distribution(state, Distribution::Ignore);
    settings.add_edge_removal_distribution(state, Distribution::Ignore);
    settings.add_birth_distribution(state, Distribution{0.1});
    settings.add_death_distribution(state, Distribution{0.1});

    auto cnw = ContactNetwork{settings};
    REQUIRE(cnw.get_births().size() == 1);
    REQUIRE(cnw.get_deaths().empty());

    cnw.create(0.0, cnw.get_births().front().identifier);
    REQUIRE(cnw.get_deaths().size() == 1);

    cnw.remove(cnw.get_deaths().front().identifier);
    REQUIRE(cnw.get_deaths().empty());
}


TEST_CASE("contact_transitions")
{
    auto settings = Settings{};
    auto const state_a = State{"a"};
    settings.add_state(state_a);
    settings.add_edge_creation_distribution(state_a, Distribution::Ignore);
    settings.add_edge_removal_distribution(state_a, Distribution::Ignore);
    auto const state_b = State{"b"};
    settings.add_state(state_b);
    settings.add_edge_creation_distribution(state_b, Distribution::Ignore);
    settings.add_edge_removal_distribution(state_b, Distribution::Ignore);
    settings.add_transition(state_a, state_b, Distribution{0.1});
    settings.add_node(state_a, 1);
    settings.add_node(state_b, 1);


    auto cnw = ContactNetwork{settings};
    auto const trans = cnw.get_transitions();

    REQUIRE(trans.size() == 1);
    cnw.change(0.0, trans.front().from, trans.front().to);

    REQUIRE(cnw.get_transitions().empty());
}


TEST_CASE("contact_interactions")
{
    auto settings = Settings{};
    auto const state_a = State{"a"};
    settings.add_state(state_a);
    settings.add_edge_creation_distribution(state_a, Distribution::Ignore);
    settings.add_edge_removal_distribution(state_a, Distribution::Ignore);
    auto const state_b = State{"b"};
    settings.add_state(state_b);
    settings.add_edge_creation_distribution(state_b, Distribution::Ignore);
    settings.add_edge_removal_distribution(state_b, Distribution::Ignore);
    settings.add_interaction(state_a, state_b, state_b, Distribution{0.1});
    settings.add_node(state_a, 1);
    settings.add_node(state_b, 1);
    settings.set_edges(1);


    auto cnw = ContactNetwork{settings};
    auto const inter = cnw.get_interactions();

    REQUIRE(inter.size() == 1);
    cnw.change(0.0, inter.front().from, inter.front().to);
    REQUIRE(cnw.get_interactions().empty());
}

