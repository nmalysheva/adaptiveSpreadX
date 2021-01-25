#include "Transformation.hpp"

#include <utility>


namespace algorithm
{

Transformation::Transformation(State state, Distribution distribution)
    : m_state{std::move(state)}, m_distribution{std::move(distribution)}
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

} // namespace algorithm

