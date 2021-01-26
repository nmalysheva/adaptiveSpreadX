#include <catch.hpp>

#include <algorithm/Settings.hpp>

using namespace algorithm;


TEST_CASE("settings_time")
{
    auto s = Settings{};
    const auto time = 0.1;
    s.set_time(time);
    REQUIRE(s.time() == time);
}


TEST_CASE("settings_birth")
{
    auto s = Settings{};
    REQUIRE(s.births().empty());
    auto const state_a = State{"S"};
    s.add_birth(state_a, Distribution{1});
    REQUIRE(s.births().size() == 1);

    auto const state_b = State{"B"};
    s.add_birth(state_b, Distribution{0});
    REQUIRE(s.births().size() == 2);

    try
    {
        s.add_birth(state_b, Distribution{0});
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string{e.what()} == Settings::DuplicateBirth);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_death")
{
    auto s = Settings{};
    REQUIRE(s.deaths().empty());
    auto const state_a = State{"S"};
    s.add_death(state_a, Distribution{1});
    REQUIRE(s.deaths().size() == 1);

    auto const state_b = State{"B"};
    s.add_death(state_b, Distribution{0});
    REQUIRE(s.deaths().size() == 2);

    try
    {
        s.add_death(state_b, Distribution{0});
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string{e.what()} == Settings::DuplicateDeath);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_transitions")
{
    auto s = Settings{};
    REQUIRE(s.transitions().empty());
    auto const state_a = State{"S"};
    auto const state_b = State{"B"};
    
    s.add_transition(state_a, state_b, Distribution{1});
    REQUIRE(s.transitions().size() == 1);

    s.add_transition(state_b, state_a, Distribution{1});
    REQUIRE(s.transitions().size() == 2);

    try
    {
        s.add_transition(state_b, state_a, Distribution{1});
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string{e.what()} == Settings::DuplicateTransition);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_interaction")
{
    auto s = Settings{};
    REQUIRE(s.interactions().empty());
    auto const state_a = State{"S"};
    auto const state_b = State{"B"};
    s.add_interaction(state_a, state_b, state_b, Distribution{1});
    REQUIRE(s.interactions().size() == 1);

    s.add_interaction(state_b, state_a, state_a, Distribution{1});
    REQUIRE(s.interactions().size() == 2);

    try
    {
        s.add_interaction(state_b, state_a, state_a, Distribution{1});
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string{e.what()} == Settings::DuplicateInteraction);
    }
    catch (...)
    {
        FAIL();
    }
}
