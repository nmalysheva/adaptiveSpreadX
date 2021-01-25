#ifndef TYPES_STATE_HPP_
#define TYPES_STATE_HPP_

#include <string>


/*!
 * \brief Representation of an individual state.
 *
 * This class wraps a type (currently `std::string`) to use as a state for
 * individuals.
 */
class State final
{
  public:
    /// type of a state
    using value_type = std::string;

    /// create a new `State` with given value
    explicit State(value_type state);

    /// explicit cast to `value_type const&`
    [[nodiscard]]
    explicit operator value_type const&() const noexcept;

  private:
    /// internal value
    value_type m_state;
};

/// compare with `State::value_type::operator==`
[[nodiscard]]
auto operator==(State const& lhs, State const& rhs) noexcept -> bool;

/// compare with `State::value_type::operator<`
[[nodiscard]]
auto operator<(State const& lhs, State const& rhs) noexcept -> bool;

#endif

