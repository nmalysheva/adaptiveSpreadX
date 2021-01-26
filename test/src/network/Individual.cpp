#include <catch.hpp>

#include <network/Individual.hpp>

using namespace network;


TEST_CASE("individual_store_value_correct")
{
    auto const s = State{"A"};
    auto const n = 0.1;
    auto const r = 0.3;

    auto const i = Individual{s, n, r};

    REQUIRE(i.state() == s);
    REQUIRE(i.new_contact_rate() == n);
    REQUIRE(i.remove_contact_rate() == r);
}

