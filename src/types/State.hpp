#ifndef TYPES_STATE_HPP_
#define TYPES_STATE_HPP_

#include <string>


class State final
{
  public:
    State() = delete;
    explicit State(std::string const& name);

    /// helper to unify Factory::make
    [[nodiscard]]
    auto operator()() const noexcept -> State const&;

    [[nodiscard]]
    auto operator==(State const& rhs) const noexcept -> bool;

    [[nodiscard]]
    auto operator<(State const& rhs) const noexcept -> bool;
  
  private:
    std::string m_name;
};

#endif

