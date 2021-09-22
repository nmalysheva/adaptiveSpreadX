#include <catch.hpp>

#include <types/NodeId.hpp>


TEST_CASE("nodeid_check_auto_increment")
{
    auto const start_id = 1 + static_cast<NodeId::id_type> (NodeId::create());
    for (auto required_id = start_id; required_id < (start_id + 10); ++required_id)
    {
        auto const node_id = NodeId::create();
        REQUIRE(static_cast<NodeId::id_type> (node_id) == required_id);
    }
}


TEST_CASE("nodeid_compare_ids")
{
    auto const id_a = NodeId::create();;
    auto const id_b = NodeId::create();

    REQUIRE(id_a == id_a);
    REQUIRE_FALSE(id_a == id_b);
    REQUIRE(id_a < id_b);
    REQUIRE_FALSE(id_b < id_a);
}


TEST_CASE("nodeid_hash")
{
    auto const id = NodeId::create();
    REQUIRE(std::hash<NodeId>{}(id) == std::hash<NodeId::id_type>{}(static_cast<NodeId::id_type> (id)));
}

