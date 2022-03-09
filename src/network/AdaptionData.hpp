#ifndef NETWORK_ADAPTIONDATA_HPP_
#define NETWORK_ADAPTIONDATA_HPP_

#include <types/State.hpp>

#include <optional>
#include <vector>


namespace network
{

/*!
 * \brief Collection of data for an adaption rule.
 *
 * If the optional `result` state is set, this adaption
 * represents a neighbour change.
 *
 * If not, it represents an edge deletion.
 */
struct AdaptionData final
{
    /// percentage of affected edges / nodes
    double percentage;

    /// the resulting state for the neighbours (optional, set if its a neighbour change)
    std::optional<State> result;

    /// optional list of States that are affected by this node-adaption rule (empty for all)
    /// (ignore if result is not set)
    std::vector<State> who;
};


/*!
 * \brief Check wheather two adaption data are equal.
 *
 * If the data represent an edge deletion they are always equal.
 * If the data represent a neighbour change they are equal if
 * - both `results` are equal
 * - the `who`s are equal (same size, same content)
 * If the data represent different kinds of adaptions they are not equal.
 */
[[nodiscard]]
inline
auto operator==(AdaptionData const& lhs, AdaptionData const& rhs) -> bool
{
    if (lhs.result and rhs.result)
    {
        return (lhs.result == rhs.result) and (lhs.who == rhs.who);
    }

    return (lhs.result.has_value() xor rhs.result.has_value()) == 0;
}

} // namespace network

#endif

