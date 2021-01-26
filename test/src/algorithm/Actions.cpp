#include <catch.hpp>

#include <algorithm/Actions.hpp>

using namespace algorithm;


TEST_CASE("actions")
{
    auto called = 0;
    auto a = [&called] () { called = 1; };
    auto b = [&called] (int const v) { called = v; };

    auto actions = Actions{};
    REQUIRE(actions.sum() == 0);
    
    actions.add(0.5, a);
    REQUIRE(actions.sum() == 0.5);

    actions.add(1, b, 2);
    REQUIRE(actions.sum() == 1.5);

    actions.call(0);
    REQUIRE(called == 1);

    actions.call(1);
    REQUIRE(called == 2);
}

