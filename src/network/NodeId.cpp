#include "NodeId.hpp"

namespace network
{

NodeId::id_type NodeId::m_next{0};


auto NodeId::create() noexcept -> NodeId
{
    return NodeId{};
}


NodeId::operator id_type() const noexcept
{
    return m_id;
}


NodeId::NodeId() noexcept
    : m_id{m_next}
{
    ++m_next;
}


auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool
{
    return static_cast<NodeId::id_type> (lhs) == static_cast<NodeId::id_type> (rhs);
}


auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool
{
    return static_cast<NodeId::id_type> (lhs) < static_cast<NodeId::id_type> (rhs);
}

} // namespace network

