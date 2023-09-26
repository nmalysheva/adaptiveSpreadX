#include "State.hpp"

#include <utility>


State::State(value_type state)
    : m_state{std::move(state)}
{
}


State::operator value_type const&() const noexcept
{
    return m_state;
}


auto operator==(State const& lhs, State const& rhs) noexcept -> bool
{
    return static_cast<State::value_type const&> (lhs) == static_cast<State::value_type const&> (rhs);
}


auto operator<(State const& lhs, State const& rhs) noexcept -> bool
{
    return static_cast<State::value_type const&> (lhs) < static_cast<State::value_type const&> (rhs);
}

