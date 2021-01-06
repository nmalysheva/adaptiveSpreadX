#include <types/NodeId.hpp>

#include <cassert>


NodeId::id_type NodeId::m_next{0};


NodeId::NodeId() noexcept
    : m_id{m_next}
{
    ++m_next;
}


NodeId::NodeId(id_type const id) noexcept
    : m_id{id}
{
    assert(m_id < m_next);
}


NodeId::operator id_type() const noexcept
{
    return m_id;
}


auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool
{
    return static_cast<NodeId::id_type> (lhs) == static_cast<NodeId::id_type> (rhs);
}


auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool
{
    return static_cast<NodeId::id_type> (lhs) < static_cast<NodeId::id_type> (rhs);
}

