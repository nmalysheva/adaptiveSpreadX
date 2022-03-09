#include <catch.hpp>

#include <network/Settings.hpp>

#include <algorithm>
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


TEST_CASE("settings_adaptions_edges")
{
    REQUIRE_THROWS_WITH(make_settings("adaption_edges_twice.txt"), Settings::DuplicateAdaption);
    REQUIRE_THROWS_WITH(make_settings("adaption_edges_unknown.txt"), Settings::UnknownState);
}


TEST_CASE("settings_adaptions_ok")
{
    auto const s = make_settings("adaption_ok.txt");
    auto count_e = 0u;
    auto count_n = 0u;

    for (auto const& elem : s.adaptions())
    {
        for (auto const adaption : elem.second)
        {
            if (adaption.result.has_value())
            {
                ++count_n;
            }
            else
            {
                ++count_e;
            }
        }
    }

    REQUIRE(count_e == 2);
    REQUIRE(count_n == 3);
}


TEST_CASE("settings_adaptions_missing_argument")
{
    REQUIRE_THROWS(make_settings("adaption_missing_argument.txt"));
}


TEST_CASE("settings_adaptions_skip_zero")
{
    auto const s = make_settings("adaption_skip_zero.txt");
    REQUIRE(s.adaptions().empty());
}

TEST_CASE("settings_adaptions_skip_no_change")
{
    auto const s = make_settings("adaption_detect_no_change.txt");
    REQUIRE(s.adaptions().size() == 1);
    REQUIRE(s.adaptions().begin()->second.size() == 1);
}


