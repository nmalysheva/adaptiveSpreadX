#ifndef SETTINGS_ALGORITHM_TRANSITION_HPP_
#define SETTINGS_ALGORITHM_TRANSITION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace settings
{

class Transition final
{
  public:
    Transition(State&& from, State&& to, Distribution const distribution) noexcept;

    auto from() const noexcept -> State const&;
    auto to() const noexcept -> State const&;

    auto distribution() const noexcept -> Distribution const&;

  private:
    State m_from;
    State m_to;
    Distribution m_distribution;
};

auto operator==(Transition const& lhs, Transition const& rhs) noexcept -> bool;

} // namespace settings

#endif

