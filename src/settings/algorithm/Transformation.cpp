#include "Transformation.hpp"


namespace settings
{

Transformation::Transformation(State&& state, Distribution const distribution) noexcept
    : m_state{std::move(state)}, m_distribution{distribution}
{
}


auto Transformation::state() const noexcept -> State const&
{
    return m_state;
}


auto Transformation::distribution() const noexcept -> Distribution const&
{
    return m_distribution;
}


auto operator==(Transformation const& lhs, Transformation const& rhs) noexcept -> bool
{
    return lhs.state() == rhs.state();
}


} // namespace settings

