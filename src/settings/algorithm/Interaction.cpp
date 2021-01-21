#include "Interaction.hpp"


namespace settings
{

Interaction::Interaction(State&& from, State&& connected, State&& to, Distribution const distribution) noexcept
    : m_from{std::move(from)}, m_connected{std::move(connected)}, m_to{std::move(to)}, m_distribution{distribution}
{
}


auto Interaction::from() const noexcept -> State const&
{
    return m_from;
}

auto Interaction::connected() const noexcept -> State const&
{
    return m_connected;
}


auto Interaction::to() const noexcept -> State const&
{
    return m_to;
}


auto Interaction::distribution() const noexcept -> Distribution const&
{
    return m_distribution;
}


auto operator==(Interaction const& lhs, Interaction const& rhs) noexcept -> bool
{
    return (lhs.from() == rhs.from()) and (lhs.connected() == rhs.connected());
}

} // namespace algorithm

