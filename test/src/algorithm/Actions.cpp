#include <catch.hpp>

#include <algorithm/Actions.hpp>

using namespace algorithm;


TEST_CASE("actions_size")
{
    auto f = [](){};
    auto actions = Actions{};

    REQUIRE(actions.count() == 0);
    actions.reserve(3);
    REQUIRE(actions.count() == 0);
    actions.add(1, f);
    REQUIRE(actions.count() == 1);
    actions.clear();
    REQUIRE(actions.count() == 0);
}


TEST_CASE("actions_perform")
{
    auto called = 'x';
    auto a = [&called] () { called = 'a'; };
    auto b = [&called] () { called = 'b'; };
    auto c = [&called] () { called = 'c'; };

    auto actions = Actions{};
    REQUIRE(actions.sum() == 0);
    REQUIRE(actions.count() == 0);

    actions.add(0.5, a);
    REQUIRE(actions.sum() == 0.5);
    actions.add(0.5, b);
    REQUIRE(actions.sum() == 1.0);
    actions.add(0.5, c);
    REQUIRE(actions.sum() == 1.5);

    actions.call(1.0);
    REQUIRE(called == 'b');

    REQUIRE(actions.count() == 2);
    REQUIRE(actions.sum() == 1.5);

    actions.call(1.0);
    REQUIRE(called == 'c');
    REQUIRE(actions.count() == 1);
    REQUIRE(actions.sum() == 0.5);

    actions.call(0.1);
    REQUIRE(called == 'a');
    REQUIRE(actions.count() == 0);
    REQUIRE(actions.sum() == 0.0);
}

