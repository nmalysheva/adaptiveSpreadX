#ifndef ALGORITHM_ACTIONS_HPP_
#define ALGORITHM_ACTIONS_HPP_

#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>


namespace algorithm
{

/// Store functions and execute the first function that exceeds a given threshold.
class Actions final
{
  public:
    /*!
     * \brief Add a new function with given propability.
     *
     *  Stores a function and assigns a threshold to it.
     *  The threshold is calculated by adding all thresholds of already added functions
     *  and the given propability.
     *
     * \tparam F function type
     * \tparam Args argument types
     *
     * \param prop propability of this function
     * \param func the function
     * \param params the parameters for the function
     */
    template <typename F,
             typename... Args> 
    auto add(double const prop, F&& func, Args&&... params) -> void
    {
        assert(prop > 0.0);
        m_total += prop;
        m_rates.push_back(m_total);
        m_actions.push_back([func, params...] () { std::invoke(func, params...); });
    }

    /// get sum of all values
    auto sum() const noexcept -> double
    {
        return m_total;
    }

    /// Execute first function that has the accumulated value higher than `value`.
    auto call(double const value) const -> void
    {
        assert(std::clamp(value, 0.0, m_total) == value);
        auto const result = std::upper_bound(m_rates.begin(), m_rates.end(), value);
        assert(result not_eq m_rates.end());
        
        auto const dist = std::distance(m_rates.begin(), result);
        m_actions[dist]();
    }

  private:
    /// sum of all given values
    double m_total{0.0};

    /// step-wise accumulated values
    std::vector<double> m_rates{};

    /// the functions
    std::vector<std::function<void(void)>> m_actions{};
};

} // namespace algorithm

#endif

