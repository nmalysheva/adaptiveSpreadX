#include <catch.hpp>

#include <network/IndividualFactory.hpp>

using namespace network;


TEST_CASE("individual_factory_make")
{
    auto settings = Settings{};
    auto const s = State{"A"};
    auto const n = 0.1;
    auto const r = 0.3;
    auto const d = 0.0;
    settings.add_state(s);
    settings.add_edge_creation_distribution(s, Distribution{n});
    settings.add_edge_removal_distribution(s, Distribution{r});
    settings.add_death_distribution(s, Distribution{d});
    settings.add_transition(s, s, Distribution{n});

    auto f = IndividualFactory{settings};
    auto const i = f.make(123.0, s);

    REQUIRE(i.modification_time == 123.0);
    REQUIRE(i.state == s);
    REQUIRE(i.new_contact_rate == n);
    REQUIRE(i.remove_contact_rate == r);
    REQUIRE(i.transition_rates.size() == 1);
}


TEST_CASE("individual_factory_birth_rates_empty")
{
    auto settings = Settings{};
    auto const s = State{"a"};
    settings.add_state(s);
    settings.add_edge_creation_distribution(s, Distribution::Ignore);
    settings.add_edge_removal_distribution(s, Distribution::Ignore);
    
    auto const f_e = IndividualFactory{settings};
    REQUIRE(f_e.draw_birth_rates().empty());

    settings.add_birth_distribution(s, Distribution::Ignore);
    auto const f_0 = IndividualFactory{settings};
    REQUIRE(f_0.draw_birth_rates().empty());
}


TEST_CASE("individual_factory_birth_rates_ok")
{
    auto settings = Settings{};
    auto const s = State{"a"};
    settings.add_state(s);
    settings.add_edge_creation_distribution(s, Distribution::Ignore);
    settings.add_edge_removal_distribution(s, Distribution::Ignore);
    settings.add_birth_distribution(s, Distribution{0.5});
    auto const f = IndividualFactory{settings};
    REQUIRE(f.draw_birth_rates().size() == 1);
    REQUIRE(f.draw_birth_rates().front().rate == 0.5);
    REQUIRE(f.draw_birth_rates().front().identifier == s);
}

    
