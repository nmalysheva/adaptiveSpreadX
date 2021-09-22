#include <catch.hpp>

#include <network/Settings.hpp>

using namespace network;


TEST_CASE("settings_state_duplicate")
{
    auto s = Settings{};
    REQUIRE_NOTHROW(s.add_state(State{"S"}));
    REQUIRE_THROWS_WITH(s.add_state(State{"S"}), Settings::DuplicateState);
}


TEST_CASE("settings_add_state_ok")
{
    auto s = Settings{};
    REQUIRE_NOTHROW(s.add_state(State{"A"}));
    REQUIRE_NOTHROW(s.add_state(State{"B"}));
    REQUIRE(s.states().size() == 2);
}


TEST_CASE("settings_edge_creation")
{
    auto s = Settings{};
    REQUIRE_THROWS(s.add_edge_creation_distribution(State{"a"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.edge_creation_distributions().empty());

    s.add_state(State{"a"});
    REQUIRE_NOTHROW(s.add_edge_creation_distribution(State{"a"}, Distribution::Ignore));
    REQUIRE(s.edge_creation_distributions().size() == 1);
    REQUIRE_THROWS_WITH(s.add_edge_creation_distribution(State{"a"}, Distribution::Ignore), Settings::DuplicateEdges);
    REQUIRE(s.edge_creation_distributions().size() == 1);
}


TEST_CASE("settings_edge_removal")
{
    auto s = Settings{};
    REQUIRE_THROWS(s.add_edge_removal_distribution(State{"a"}, Distribution::Ignore));
    REQUIRE(s.edge_removal_distributions().empty());

    s.add_state(State{"a"});
    REQUIRE_NOTHROW(s.add_edge_removal_distribution(State{"a"}, Distribution::Ignore));
    REQUIRE(s.edge_removal_distributions().size() == 1);
    REQUIRE_THROWS_WITH(s.add_edge_removal_distribution(State{"a"}, Distribution::Ignore), Settings::DuplicateEdges);
    REQUIRE(s.edge_removal_distributions().size() == 1);
}


TEST_CASE("settings_death")
{
    auto s = Settings{};
    REQUIRE_THROWS_WITH(s.add_death_distribution(State{"a"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.death_distributions().empty());

    s.add_state(State{"a"});
    REQUIRE_NOTHROW(s.add_death_distribution(State{"a"}, Distribution::Ignore));
    REQUIRE(s.death_distributions().size() == 1);
    REQUIRE_THROWS_WITH(s.add_death_distribution(State{"a"}, Distribution::Ignore), Settings::DuplicateDeath);
    REQUIRE(s.death_distributions().size() == 1);
}


TEST_CASE("settings_birth")
{
    auto s = Settings{};
    REQUIRE_THROWS_WITH(s.add_birth_distribution(State{"a"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.birth_distributions().empty());

    s.add_state(State{"a"});
    REQUIRE_NOTHROW(s.add_birth_distribution(State{"a"}, Distribution::Ignore));
    REQUIRE(s.birth_distributions().size() == 1);
    REQUIRE_THROWS_WITH(s.add_birth_distribution(State{"a"}, Distribution::Ignore), Settings::DuplicateBirth);
    REQUIRE(s.birth_distributions().size() == 1);
}


TEST_CASE("settings_initial_nodes")
{
    auto s = Settings{};
    REQUIRE_THROWS_WITH(s.add_node(State{"a"}, 0), Settings::UnknownState);
    REQUIRE(s.nodes().empty());

    s.add_state(State{"a"});
    REQUIRE_NOTHROW(s.add_node(State{"a"}, 100));
    REQUIRE(s.nodes().size() == 1);
    REQUIRE_THROWS_WITH(s.add_node(State{"a"}, 1), Settings::DuplicateCount);
    REQUIRE(s.nodes().size() == 1);
}


TEST_CASE("settings_initial_edges")
{
    auto s = Settings{};
    REQUIRE(s.edges() == 0);
    REQUIRE_NOTHROW(s.set_edges(100));
    REQUIRE(s.edges() == 100);
    REQUIRE_THROWS_WITH(s.set_edges(1), Settings::DuplicateEdgeInit);
}


TEST_CASE("settings_transitions")
{
    auto s = Settings{};
    REQUIRE(s.transitions().empty());

    s.add_state(State{"a"});
    REQUIRE_THROWS_WITH(s.add_transition(State{"u"}, State{"a"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.transitions().empty());
    REQUIRE_THROWS_WITH(s.add_transition(State{"a"}, State{"u"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.transitions().empty());
    REQUIRE_NOTHROW(s.add_transition(State{"a"}, State{"a"}, Distribution::Ignore));
    REQUIRE(s.transitions().size() == 1);
    REQUIRE_THROWS_WITH(s.add_transition(State{"a"}, State{"a"}, Distribution::Ignore), Settings::DuplicateTransition);
}


TEST_CASE("settings_interactions")
{
    auto s = Settings{};
    REQUIRE(s.interactions().empty());

    s.add_state(State{"a"});
    REQUIRE_THROWS_WITH(s.add_interaction(State{"u"}, State{"a"}, State{"a"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.transitions().empty());
    REQUIRE_THROWS_WITH(s.add_interaction(State{"a"}, State{"u"}, State{"a"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.transitions().empty());
    REQUIRE_THROWS_WITH(s.add_interaction(State{"a"}, State{"a"}, State{"u"}, Distribution::Ignore), Settings::UnknownState);
    REQUIRE(s.transitions().empty());
    REQUIRE_NOTHROW(s.add_interaction(State{"a"}, State{"a"}, State{"a"}, Distribution::Ignore));
    REQUIRE(s.interactions().size() == 1);
    REQUIRE_THROWS(s.add_interaction(State{"a"}, State{"a"}, State{"a"}, Distribution::Ignore), Settings::DuplicateInteraction);
}

