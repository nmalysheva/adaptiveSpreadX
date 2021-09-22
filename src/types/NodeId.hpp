#ifndef TYPES_NODEID_HPP_
#define TYPES_NODEID_HPP_

#include <cstddef>
#include <functional>


/*!
 * \brief Node identifier class
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
    id_type m_id{m_next};
    
    /// Construct with the next unused id.
    NodeId() noexcept;
};
    

/// compare with `NodeId::id_type::operator==`
[[nodiscard]]
auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;

/// compare with `NodeId::id_type::operator<`
[[nodiscard]]
auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;


namespace std
{
/// make NodeId available for std::hash
template <>
struct hash<NodeId>
{
    auto operator()(NodeId const id) const -> std::size_t
    {
        return std::hash<NodeId::id_type>{}(static_cast<NodeId::id_type> (id));
    }
};
}

#endif

