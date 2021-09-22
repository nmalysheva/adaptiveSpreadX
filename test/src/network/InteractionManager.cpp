#include <catch.hpp>

#include <network/InteractionManager.hpp>

using network::InteractionManager;
using network::Settings;


TEST_CASE("interactionmanager_empty")
{
    auto s = Settings{};
    auto im = InteractionManager{s};

    REQUIRE(im.get_rates().empty());
}


TEST_CASE("interactionmanager_get_state")
{
    auto s = Settings{};

    auto const sa = State{"a"};
    auto const sb = State{"b"};
   
    s.add_state(sa); 
    s.add_state(sb);
    s.add_interaction(sa, sb, sa, Distribution::Ignore);
    s.add_interaction(sb, sa, sb, Distribution::Ignore);
    
    auto im = InteractionManager{s};

    REQUIRE(im.get_state(sa, sb) == sa);
    REQUIRE(im.get_state(sb, sa) == sb);
}


TEST_CASE("interactionmanager_add_and_remove_nodes")
{
    auto s = Settings{};

    auto const sa = State{"a"};
    auto const sb = State{"b"};
    auto const sc = State{"c"};
   
    s.add_state(sa); 
    s.add_state(sb);
    s.add_state(sc);
    s.add_interaction(sa, sb, sa, Distribution{0.5});
    s.add_interaction(sb, sa, sb, Distribution{0.1});
    s.add_interaction(sa, sc, sa, Distribution{0.2});
    
    auto im = InteractionManager{s};

    auto const first = NodeId::create();
    auto const second = NodeId::create();
    auto const third = NodeId::create();

    im.add(first, second, sa, sb);
    REQUIRE(im.get_rates().size() == 2);

    im.add(first, third, sa, sc);
    REQUIRE(im.get_rates().size() == 3);

    im.remove(first, third);
    REQUIRE(im.get_rates().size() == 2);

    im.remove(second);
    REQUIRE(im.get_rates().empty());
}

