#include <catch.hpp>

#include <settings/Settings.hpp>

using namespace settings;
using test_container = std::unordered_map<std::string, std::vector<std::string>>;


TEST_CASE("settings_no_species")
{
    auto const tc = test_container{};
    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == Settings::NoSpecies);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_no_time")
{
    auto const tc = test_container{{"Species", {"S 0 0"}}};
    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == Settings::NoTime);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_time_twice")
{
    auto const tc = test_container{
        {"Species", {"S 0 0"}},
        {"Time", {"1", "2"}}
    };

    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == Settings::DuplicateTime);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_empty_section")
{
    auto const tc = test_container{
        {"Species", {"S 0 0"}},
        {"Time", {"1"}},
        {"Births", {}}
    };

    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == std::string{Settings::EmptySection} + "Births");
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_unknown_section")
{
    auto const tc = test_container{
        {"Species", {"S 0 0"}},
        {"Time", {"1"}},
        {"foo", {"bar"}}
    };

    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == std::string{Settings::UnknownSection} + "foo");
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_unknown_state")
{
    auto const tc = test_container{
        {"Species", {"S 0 0"}},
        {"Time", {"1"}},
        {"Births", {"I 0"}}
    };

    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == std::string{Settings::UnknownState} + "Births");
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_edges_twice")
{
    auto const tc = test_container{
        {"Species", {"S 0 0"}},
        {"Time", {"1"}},
        {"Edges", {"1", "2"}}
    };

    try
    {
        auto const s = Settings{tc};
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == Settings::DuplicateEdges);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("settings_all_good")
{
    auto const tc = test_container{
        {"Species", {"S 0 0"}},
        {"Time", {"10"}},
        {"Nodes", {"S 1"}},
        {"Edges", {"2"}},
        {"Births", {"S 1"}},
        {"Deaths", {"S 1"}},
        {"Transitions", {"S S 1"}},
        {"Interactions", {"S S S 1"}}
    };

    auto const s = Settings{tc};
    REQUIRE(s.algorithm().time() == 10);
    REQUIRE(s.network().edges() == 2);
}

