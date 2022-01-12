#include <catch.hpp>

#include <network/InteractionManager.hpp>

#include <fstream>


using network::InteractionManager;
using network::Settings;


TEST_CASE("interactionmanager")
{
    auto fs = std::ifstream{std::string{TEST_CONFIG_FOLDER} + "/network/ok.txt"};
    auto const config = configuration::Configuration{fs};
    auto const s = Settings{config};
    auto im = InteractionManager{s};

    REQUIRE(im.get_rates().empty());

    // 2 entries in the config
    REQUIRE(im.max_rates().size() == 2);

    auto const a = State{"A"};
    auto const b = State{"B"};
    REQUIRE(im.get_state(a, a) == b);
    REQUIRE(im.get_state(a, b) == b);

    auto const n_a = NodeId::create();
    auto const n_b = NodeId::create();

    im.add(n_a, n_b, a, b);
    REQUIRE(im.get_rates().size() == 1);

    im.remove(n_b, n_a);
    REQUIRE(im.get_rates().empty());

    im.add(n_a, n_b, a, b);
    REQUIRE(im.get_rates().size() == 1);

    im.remove(n_a);
    REQUIRE(im.get_rates().empty());
}

