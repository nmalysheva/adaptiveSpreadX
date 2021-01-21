#ifndef SETTINGS_ALGORITHM_TRANSOFRMATION_HPP_
#define SETTINGS_ALGORITHM_TRANSOFRMATION_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace settings
{

class Transformation final
{
  public:
    Transformation(State&& state, Distribution const distribution) noexcept;

    auto state() const noexcept -> State const&;

    auto distribution() const noexcept -> Distribution const&;

  private:
    State m_state;
    Distribution m_distribution;
};


auto operator==(Transformation const& lhs, Transformation const& rhs) noexcept -> bool;

} // namespace settings

#endif

