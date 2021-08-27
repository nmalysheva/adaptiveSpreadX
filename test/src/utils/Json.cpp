#include <catch.hpp>

#include <utils/Json.hpp>

#include <vector>


TEST_CASE("json_implode")
{
    auto v = std::vector<int>{1,2,3};
    auto r = utils::json::implode(v);
    REQUIRE(r == "1,2,3");

    v.clear();
    r = utils::json::implode(v);
    REQUIRE(r.empty());
}

TEST_CASE("json_block")
{
    auto block = utils::json::Block{};
    REQUIRE(block.to_string() == "{}");

    block.add_number("a", 1);
    REQUIRE(block.to_string() == "{\"a\":1}");

    block.add_string("b", "c");
    REQUIRE(block.to_string() == "{\"a\":1,\"b\":\"c\"}");

    block.add_json("d", "e");
    REQUIRE(block.to_string() == "{\"a\":1,\"b\":\"c\",\"d\":e}");
}


TEST_CASE("json_list")
{
    auto list = utils::json::List<int>{};
    REQUIRE(list.to_string() == "[]");

    list.add(1);
    REQUIRE(list.to_string() == "[1]");

    list.add(2);
    REQUIRE(list.to_string() == "[1,2]");
}

