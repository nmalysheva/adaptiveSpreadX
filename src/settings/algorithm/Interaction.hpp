#ifndef SETTINGS_ALGORITHM_INTERACTION_HPP_
#define SETTINGS_ALGORITHM_INTERACTION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace settings
{

class Interaction final
{
  public:
    Interaction(State&& from, State&& connected, State&& to, Distribution const distribution) noexcept;

    auto from() const noexcept -> State const&;
    auto connected() const noexcept -> State const&;
    auto to() const noexcept -> State const&;
    auto distribution() const noexcept -> Distribution const&;

  private:
    State m_from;
    State m_connected;
    State m_to;
    Distribution m_distribution;
};

auto operator==(settings::Interaction const& lhs, settings::Interaction const& rhs) noexcept -> bool;

} // namespace algorithm

#endif

