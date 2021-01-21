#include "Transition.hpp"


namespace settings
{

Transition::Transition(State&& from, State&& to, Distribution const distribution) noexcept
    : m_from{std::move(from)}, m_to{std::move(to)}, m_distribution{distribution}
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


} // namespace settings

