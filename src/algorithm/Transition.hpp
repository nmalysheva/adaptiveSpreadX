#ifndef ALGORITHM_TRANSITION_HPP_
#define ALGORITHM_TRANSITION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace algorithm
{

/*!
 * \brief Representation of a transition.
 *
 * A transition changes a state into another with a given propability,
 * drawn from a distribution.
 */
class Transition final
{
  public:
    /// Transition `from` to `to`, with propability drawn from `distribution`.
    explicit Transition(State from, State to, Distribution distribution);

    /// get state to change
    [[nodiscard]]
    auto from() const noexcept -> State const&;

    /// get target state
    [[nodiscard]]
    auto to() const noexcept -> State const&;

    /// get distribution of transition
    [[nodiscard]]
    auto distribution() const noexcept -> Distribution const&;

  private:
    /// state affected
    State m_from;

    /// resulting state
    State m_to;

    /// distribution for propability generation
    Distribution m_distribution;
};

/// `Transition`s are equal if their `from`s and `to`s equal
[[nodiscard]]
auto operator==(Transition const& lhs, Transition const& rhs) noexcept -> bool;

} // namespace algorithm

#endif

