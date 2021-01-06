#include <catch.hpp>

#include <types/NodeId.hpp>


TEST_CASE("check_auto_increment")
{
    for (auto required_id = NodeId::id_type{0}; required_id < 10; ++required_id)
    {
        auto const node_id = NodeId::create();
        REQUIRE(static_cast<NodeId::id_type> (node_id) == required_id);
    }
}


TEST_CASE("compare_ids")
{
    auto const id_a = NodeId::create();;
    auto const id_b = NodeId::refer(static_cast<NodeId::id_type> (id_a));
    auto const id_c = NodeId::create();

    REQUIRE(id_a == id_b);
    REQUIRE_FALSE(id_a == id_c);
    REQUIRE(id_a < id_c);
    REQUIRE_FALSE(id_c < id_a);
}

