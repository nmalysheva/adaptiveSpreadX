#include <catch.hpp>

#include <network/Settings.hpp>

#include <fstream>
#include <string>


using namespace network;
using namespace configuration;


auto make_settings(std::string const& name)
{
    auto const PATH = std::string{TEST_CONFIG_FOLDER} + "/network/" + name;
    auto fs = std::ifstream{PATH};
    return Settings{Configuration{fs}};
}



TEST_CASE("settings_network_mising")
{
    REQUIRE_THROWS_WITH(make_settings("network_missing.txt"), Settings::NetworkMissing);
}


TEST_CASE("settings_state_duplicate")
{
    REQUIRE_THROWS_WITH(make_settings("state_duplicate.txt"), Settings::DuplicateState);
}


TEST_CASE("settings_network_ok")
{
    auto const s = make_settings("ok.txt");
    REQUIRE(s.states().size() == 2);
    REQUIRE(s.edges() == 1);
    REQUIRE(s.nodes().size() == 2);
    REQUIRE(s.seed() == 123);
}


TEST_CASE("settings_edges_twice")
{
    REQUIRE_THROWS_WITH(make_settings("edges_twice.txt"), Settings::DuplicateEdgeInit);
}

TEST_CASE("settings_seed_twice")
{
    REQUIRE_THROWS_WITH(make_settings("seed_twice.txt"), Settings::DuplicateSeedInit);
}


TEST_CASE("settings_birth")
{
    REQUIRE_THROWS_WITH(make_settings("birth_twice.txt"), Settings::DuplicateBirth);
    REQUIRE_THROWS_WITH(make_settings("birth_unknown.txt"), Settings::UnknownState);
    
    auto const s = make_settings("ok.txt");
    REQUIRE(s.birth_distributions().size() == 1);
}


TEST_CASE("settings_deaths")
{
    REQUIRE_THROWS_WITH(make_settings("death_twice.txt"), Settings::DuplicateDeath);
    REQUIRE_THROWS_WITH(make_settings("death_unknown.txt"), Settings::UnknownState);
    
    auto const s = make_settings("ok.txt");
    REQUIRE(s.death_distributions().size() == 1);
}


TEST_CASE("settings_edges")
{
    REQUIRE_THROWS_WITH(make_settings("add_edge_twice.txt"), Settings::DuplicateEdge);
    REQUIRE_THROWS_WITH(make_settings("add_edge_unknown.txt"), Settings::UnknownState);
    
    REQUIRE_THROWS_WITH(make_settings("remove_edge_twice.txt"), Settings::DuplicateEdge);
    REQUIRE_THROWS_WITH(make_settings("remove_edge_unknown.txt"), Settings::UnknownState);
    
    auto const s = make_settings("ok.txt");
    REQUIRE(s.edge_creation_distributions().size() == 2);
    REQUIRE(s.edge_removal_distributions().size() == 2);
}

TEST_CASE("settings_transitions")
{
    REQUIRE_THROWS_WITH(make_settings("transition_twice.txt"), Settings::DuplicateTransition);
    REQUIRE_THROWS_WITH(make_settings("transition_unknown_a.txt"), Settings::UnknownState);
    REQUIRE_THROWS_WITH(make_settings("transition_unknown_b.txt"), Settings::UnknownState);

    auto const s = make_settings("ok.txt");
    REQUIRE(s.transitions().size() == 2);
}


TEST_CASE("settings_interactions")
{
    REQUIRE_THROWS_WITH(make_settings("interaction_twice.txt"), Settings::DuplicateInteraction);
    REQUIRE_THROWS_WITH(make_settings("interaction_unknown_a.txt"), Settings::UnknownState);
    REQUIRE_THROWS_WITH(make_settings("interaction_unknown_b.txt"), Settings::UnknownState);
    REQUIRE_THROWS_WITH(make_settings("interaction_unknown_c.txt"), Settings::UnknownState);

    auto const s = make_settings("ok.txt");
    REQUIRE(s.interactions().size() == 2);
}


TEST_CASE("settings_quarantines")
{
    REQUIRE_THROWS_WITH(make_settings("quarantine_twice.txt"), Settings::DuplicateQuarantine);
    REQUIRE_THROWS_WITH(make_settings("quarantine_unknown.txt"), Settings::UnknownState);
    
    auto const s = make_settings("ok.txt");
    REQUIRE(s.quarantines().size() == 2);
}

