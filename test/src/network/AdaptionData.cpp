#include <catch.hpp>

#include <network/AdaptionData.hpp>


using namespace network;


TEST_CASE("adaption_compare_edges")
{
    auto const a = AdaptionData{0.1, std::nullopt, {}};
    auto const b = AdaptionData{0.9, std::nullopt, {}};

    REQUIRE(a == b);
}


TEST_CASE("adaption_compare_mix")
{
    auto const a = AdaptionData{0.1, std::nullopt, {}};
    auto const b = AdaptionData{0.9, State{"X"}, {}};

    REQUIRE_FALSE(a == b);
}


TEST_CASE("adaption_compare_neighbours")
{
    auto a = AdaptionData{0.1, State{"X"}, {}};
    auto b = AdaptionData{0.9, State{"X"}, {}};
    REQUIRE(a == b);

    a.who.push_back(State{"A"});
    a.who.push_back(State{"B"});
    REQUIRE_FALSE(a == b);
    
    b.who.push_back(State{"A"});
    b.who.push_back(State{"B"});
    REQUIRE(a == b);
}
