#include <types/NodeId.hpp>


NodeId::NodeId(id_type const id) noexcept
    : m_id{id}
{
}


NodeId::operator id_type() const noexcept
{
    return m_id;
}


auto NodeId::operator++() noexcept -> NodeId&
{
    ++m_id;
    return *this;
}


auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool
{
    return static_cast<NodeId::id_type> (lhs) == static_cast<NodeId::id_type> (rhs);
}


auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool
{
    return static_cast<NodeId::id_type> (lhs) < static_cast<NodeId::id_type> (rhs);
}

