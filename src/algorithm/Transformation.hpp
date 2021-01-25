#ifndef ALGORITHM_TRANSOFRMATION_HPP_
#define ALGORITHM_TRANSOFRMATION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace algorithm
{

/*!
 * \brief Representation of a transformation.
 *
 * A transformation describes the birth / death of a node with the given state.
 *
 * \note It is up to the user to distinguish between birth and death.
 */
class Transformation final
{
  public:
    /// Transformation from / to `state`, with propability drawn from `distribution`.
    explicit Transformation(State state, Distribution distribution);

    /// get state of the transformation
    auto state() const noexcept -> State const&;

    /// get distribution of transformation
    auto distribution() const noexcept -> Distribution const&;

  private:
    /// state of transformation
    State m_state;

    /// distribution for propability generation
    Distribution m_distribution;
};

/// `Transformation`s are equal if their `state`s equal.
auto operator==(Transformation const& lhs, Transformation const& rhs) noexcept -> bool;

} // namespace settings

#endif

