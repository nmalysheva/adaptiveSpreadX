#ifndef NETWORK_NODEID_HPP_
#define NETWORK_NODEID_HPP_

#include <cstddef>
#include <functional>


namespace network
{

/*!
 * \brief node identifier class
 *
 * This class wraps an immutable id of an integral type (\see NodeId::id_type).
 *
 * Use \see `create()` to create a new object with an unused id.
 */
class NodeId final
{
  public:
    /// type of an id
    using id_type = std::size_t;

    /// construct with the next unused id
    [[nodiscard]]
    static auto create() noexcept -> NodeId;

    /// explicit cast to underlaying type
    [[nodiscard]]
    explicit operator id_type() const noexcept;

  private:
    /// id of the next object created
    static id_type m_next;

    /// id of this object
    id_type const m_id;
    
    /// Construct with the next unused id.
    NodeId() noexcept;
};
    

/// compare with `NodeId::id_type::operator==`
auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;

/// compare with `NodeId::id_type::operator<`
auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;

} // namespace network

/// make NodeId available for std::hash
namespace std
{
template <>
struct hash<network::NodeId>
{
    auto operator()(network::NodeId const id) const -> std::size_t
    {
        return std::hash<network::NodeId::id_type>{}(static_cast<network::NodeId::id_type> (id));
    }
};
}

#endif

