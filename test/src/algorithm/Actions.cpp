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


TEST_CASE("actions_call")
{
    auto called = 'x';
    auto a = [&called] () { called = 'a'; };
    auto b = [&called] () { called = 'b'; };

    auto actions = Actions{};

    actions.add(1.0, a);
    REQUIRE(actions.sum() == 1.0);
    actions.add(1.0, b);
    REQUIRE(actions.sum() == 2.0);

    actions.call(0.0);
    REQUIRE(called == 'a');
    actions.call(0.5);
    REQUIRE(called == 'a');
    actions.call(0.1);
    REQUIRE(called == 'a');

    actions.call(1.1);
    REQUIRE(called == 'b');
    actions.call(1.5);
    REQUIRE(called == 'b');
    actions.call(2.0);
    REQUIRE(called == 'b');
}


TEST_CASE("actions_call_and_remove")
{
    auto called = 0.0;
    auto a = [&called] () { called += 1.0; };
    auto b = [&called] () { called += 2.0; };
    auto c = [&called] () { called += 4.0; };
    auto d = [&called] () { called += 8.0; };
    auto e = [&called] () { called += 16.0; };


    auto actions = Actions{};
    actions.add(1.0, a);
    actions.add(1.0, b);
    actions.add(1.0, c);
    actions.add(1.0, d);
    actions.add(1.0, e);

    actions.call_and_remove(3.0); // middle
    actions.call_and_remove(1.0); // first
    actions.call_and_remove(2.0); // second to last
    actions.call_and_remove(2.0); // last
    actions.call_and_remove(1.0); // first/last

    REQUIRE(actions.sum() == 0.0);
    REQUIRE(called == 31.0);
}

