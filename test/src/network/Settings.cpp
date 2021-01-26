#include <catch.hpp>

#include <network/Settings.hpp>

using namespace network;


TEST_CASE("settings_edges")
{
    auto s = Settings{};
    REQUIRE(s.edges() == 0);
    s.set_edges(1);
    REQUIRE(s.edges() == 1);
}

TEST_CASE("settings_states")
{
    auto s = Settings{};
    REQUIRE(s.states().empty());

    auto const state = State{"X"};
    auto const state_b = State{"A"};
    s.add_factory(state, Distribution{0}, Distribution{0});
    REQUIRE(s.states().size() == 1);
    REQUIRE(s.factory().make(state).state() == state);
    REQUIRE(s.states().count(state) == 1);

    s.add_factory(state_b, Distribution{0}, Distribution{0});
    REQUIRE(s.states().size() == 2);
    REQUIRE(s.factory().make(state_b).state() == state_b);
    REQUIRE(s.states().count(state_b) == 1);

    try
    {
        s.add_factory(state, Distribution{1}, Distribution{1});
        FAIL();
    }
    catch (std::logic_error const&)
    {
        REQUIRE(s.states().size() == 2);
        REQUIRE(s.factory().make(state).state() == state);
        REQUIRE(s.factory().make(state_b).state() == state_b);
        REQUIRE(s.states().count(state) == 1);
        REQUIRE(s.states().count(state_b) == 1);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_nodes")
{
    auto s = Settings{};
    auto const state = State{"x"};

    try
    {
        s.add_node(state, 0);
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string{e.what()} == Settings::UnknownState);
    }
    catch (...)
    {
        FAIL();
    }

    s.add_factory(state, Distribution{0}, Distribution{0});
    s.add_node(state, 100);
    REQUIRE(s.nodes().size() == 1);
    REQUIRE(s.nodes().front().first == state);
    REQUIRE(s.nodes().front().second == 100);

    try
    {
        s.add_node(state, 1);
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string{e.what()} == Settings::DuplicateState);
    }
    catch (...)
    {
        FAIL();
    }
}

