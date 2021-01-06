#ifndef TYPES_NODEID_HPP_
#define TYPES_NODEID_HPP_

/*! \file
 * \author Mathias Lindemann
 *
 * Contains the node identifier class.
 */

#include <cstdint>
#include <functional>

/*!
 * \brief Node identifier class
 *
 * This class wraps an immutable id of an integral type (see NodeId::id_type).
 *
 * Use the default consructor to create an object of this class with a new and unused id.
 * This should be used to populate data structures.
 * Use a value to create an object to refer to an already created object with the same id.
 * This should be used to access the correct object in the data structure.
 * \note In debug mode a check is performed whether the given id was already used. In release mode
 * (-DNDEBUG) this check is removed and the user has to ensure this constraint.
 */
class NodeId final
{
  public:
    /// Type of an id.
    using id_type = std::uint64_t;

    /// Construct with the next unused id.
    NodeId() noexcept;

    /// Initialise with given id.
    explicit NodeId(id_type const id) noexcept;

    /// Convert to encapsulated type.
    explicit operator id_type() const noexcept;

  private:
    /// id of the next object created
    static id_type m_next;

    /// id of this object
    id_type const m_id;
};
    

/// NodeIds are equal if their ids are equal.
auto operator==(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;

/// Compare with id_type::operator<
auto operator<(NodeId const& lhs, NodeId const& rhs) noexcept -> bool;


/// Make NodeId available for std::hash
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

