#include "Transition.hpp"

#include <utility>


namespace algorithm
{

Transition::Transition(State from, State to, Distribution distribution)
    : m_from{std::move(from)}, m_to{std::move(to)}, m_distribution{std::move(distribution)}
{
}

auto Transition::from() const noexcept -> State const&
{
    return m_from;
}


auto Transition::to() const noexcept -> State const&
{
    return m_to;
}


auto Transition::distribution() const noexcept -> Distribution const&
{
    return m_distribution;
}


auto operator==(Transition const& lhs, Transition const& rhs) noexcept -> bool
{
    return (lhs.from() == rhs.from()) and (lhs.to() == rhs.to());
}

} // namespace algorithm

