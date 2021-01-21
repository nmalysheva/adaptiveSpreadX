#ifndef TYPES_NODEID_HPP_
#define TYPES_NODEID_HPP_

#include <cstddef>
#include <functional>

/*!
 * \brief Node identifier class
 *
 * This class wraps an immutable id of an integral type (\see NodeId::id_type).
 *
 * Use \see create() to create a new object with an unused id.
 */
class NodeId final
{
  public:
    /// Type of an id.
    using id_type = std::size_t;

    /// Construct with the next unused id.
    [[nodiscard]]
    static auto create() noexcept -> NodeId;

    /// Convert to encapsulated type.
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
    

/// NodeIds are equal if their ids are equal.
auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;

/// Compare with id_type::operator<
auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;


/// make NodeId available for std::hash
namespace std
{
template <>
struct hash<NodeId>
{
    auto operator()(NodeId const& id) const noexcept -> std::size_t
    {
        return std::hash<NodeId::id_type>{}(static_cast<NodeId::id_type> (id));
    }
};
}

#endif

