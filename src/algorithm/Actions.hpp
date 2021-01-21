#ifndef ALGORITHM_ACTIONS_HPP_
#define ALGORITHM_ACTIONS_HPP_

#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>


namespace algorithm
{

class Actions final
{
  public:
    template <typename F,
             typename... Args> 
    auto push_back(double const prop, F&& func, Args&&... params) -> void
    {
        assert(prop > 0.0);
        m_total += prop;
        m_rates.push_back(m_total);
        m_actions.push_back([func, params...] () { std::invoke(func, params...); });
    }

    auto sum() const noexcept -> double
    {
        return m_total;
    }

    auto call(double const value) const -> void
    {
        assert(std::clamp(value, 0.0, m_total) == value);
        auto const result = std::upper_bound(m_rates.begin(), m_rates.end(), value);
        assert(result not_eq m_rates.end());
        
        auto const dist = std::distance(m_rates.begin(), result);
        m_actions[dist]();
    }

  private:
    double m_total{0.0};
    std::vector<double> m_rates{};
    std::vector<std::function<void(void)>> m_actions{};
};

} // namespace algorithm

#endif

