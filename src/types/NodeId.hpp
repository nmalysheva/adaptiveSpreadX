#ifndef TYPES_NODEID_HPP_
#define TYPES_NODEID_HPP_

/*! \file
 * \author Mathias Lindemann
 *
 * Contains the identifier class.
 */

#include <cstdint>
#include <functional>
#include <limits>

/*!
 * \brief Identifier class
 *
 * The class is just an alias for any ID type (e.g. int).
 * Its only purpose, beside explicit usage, is to ensure that no
 * operations on the value are performed.
 */
class NodeId final
{
  public:
    /// Type of an ID.
    using id_type = std::uintmax_t;

    /// Construct an invalid ID
    NodeId() noexcept = default;

    /// Initialise with given id.
    explicit NodeId(id_type const id) noexcept;

    /// Convert to encapsulated type.
    explicit operator id_type() const noexcept;

    /// Increase id by 1
    auto operator++() noexcept -> NodeId&;

  private:
    /// Invalid Id
    static id_type const  INVALID{std::numeric_limits<id_type>::max()};

    /// Stored id.
    id_type m_id{0};
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

