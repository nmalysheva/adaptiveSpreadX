#ifndef NETWORK_INTERACTION_HPP_
#define NETWORK_INTERACTION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>

#include <utility>


namespace network
{

/*!
 * \brief Representation of an interaction.
 *
 * An interaction changes a state into antoher one with a given propability, drawn from a distribution,
 * if the individual of this state is connected to an individual with a required state.
 */
struct Interaction final
{
  public:
    /// Interaction: `from` connected to `connected`, resulting in `to`, with given propability drawn from `distribution`.
    explicit Interaction(State from, State connected, State to, Distribution distribution)
        : from{std::move(from)}, connected{std::move(connected)}, to{std::move(to)}, distribution{std::move(distribution)}
    {
    }

    /// affected state
    State from;

    /// required connection
    State connected;

    /// result state
    State to;

    /// distribution for propability generation
    Distribution distribution;
};



/*!
 * \brief Compare two Interactions by comparing the `from` and `connected` states.
 *
 * It is compared by:
 * - `lhs.from < rhs.from == true` this comparison is `true`
 * - if `lhs.from == rhs.from` the result depends on the order of the `connected`s
 * - otherwise it is `false`
 *
 * \param lhs left hand side of the comparison
 * \param rhs right hand side of the comparison
 *
 * \return result, see description of this function
 */ 
[[nodiscard]]
auto operator<(Interaction const& lhs, Interaction const& rhs) noexcept -> bool;

} // namespace network

#endif

