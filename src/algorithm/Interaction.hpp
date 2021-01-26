#ifndef ALGORITHM_INTERACTION_HPP_
#define ALGORITHM_INTERACTION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace algorithm
{

/*!
 * \brief Representation of an interaction.
 *
 * An interaction changes a state into antoher one with a given propability, drawn from a distribution,
 * if the individual of this state is connected to an individual with a required state.
 */
class Interaction final
{
  public:
    /// Interaction: `from` connected to `connected`, resulting in `to`, with given propability drawn from `distribution`.
    explicit Interaction(State from, State connected, State to, Distribution distribution);

    /// get state to change
    [[nodiscard]]
    auto from() const noexcept -> State const&;

    /// get required connected state
    [[nodiscard]]
    auto connected() const noexcept -> State const&;

    /// get resulting state
    [[nodiscard]]
    auto to() const noexcept -> State const&;

    /// get distribution of reaction
    [[nodiscard]]
    auto distribution() const noexcept -> Distribution const&;

  private:
    /// affected state
    State m_from;

    /// required connection
    State m_connected;

    /// result state
    State m_to;

    /// distribution for propability generation
    Distribution m_distribution;
};

/// `Interaction`s are equal if their `from`s and `connected`s equal
[[nodiscard]]
auto operator==(Interaction const& lhs, Interaction const& rhs) noexcept -> bool;

} // namespace algorithm

#endif

