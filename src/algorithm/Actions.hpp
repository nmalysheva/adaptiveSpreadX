#ifndef ALGORITHM_ACTIONS_HPP_
#define ALGORITHM_ACTIONS_HPP_

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <vector>


namespace algorithm
{

/// Store functions and execute the first function that exceeds a given threshold.
class Actions final
{
  public:
    using function_type = std::function<void(void)>;

    /*!
     * \brief Add a new function with given propability.
     *
     *  Stores a function and assigns a threshold to it.
     *  The threshold is calculated by adding all thresholds of already added functions
     *  and the given propensity.
     *
     * \tparam F function type
     * \tparam Args argument types
     *
     * \param prop propensity of this function
     * \param func the function
     * \param params the parameters for the function
     */
    template <typename F,
              typename... Args> 
    auto add(double const prop, F&& func, Args&&... params) -> void
    {
        assert(prop > 0.0);

        m_sum += prop;
        m_actions.emplace_back(m_sum, [func, params...] () { std::invoke(func, params...); });
    }

    /// get sum of all values
    [[nodiscard]]
    auto sum() const noexcept -> double
    {
        return m_sum;
    }

    /// get number of actions
    [[nodiscard]]
    auto count() const noexcept -> std::size_t
    {
        assert(m_actions.size() >= m_performed);
        return m_actions.size() - m_performed;
    }

    /*!
     * \brief Execute first function that has the accumulated value not less than `value`.
     *
     * After the execution of the function the next function with a higher threshold is
     * searched. All entries from the executed function until one-before the found function
     * are set to the found function.
     * If no function is found (i.e. the executed function was the one with the highes threshold)
     * it is removed.
     *
     * This approach allows to remove executed functions from the internal vector without
     * altering its size. (erase does remove an element but does not free the memeory)
     *
     * \note The propensity of the function with the next higher threshold is taken into account more
     * on the next call. This is accepted because the reuse of the actions is done only in SSATAN-X, 
     * which is an approximation itself.
     *
     * \note If the last action was performed, sum() will change. Therefore if no new action is added
     * to actions directly a call to sum() is required to make sure the next call is in the valid range.
     *
     * \param value the threshold
     */
    auto call(double const value) -> void
    {
        assert(std::clamp(value, 0.0, m_sum) == value);

        auto const comp_lower = [](auto const& action, auto const prop)
        {
            return action.first < prop;
        };
        auto result = std::lower_bound(m_actions.begin(), m_actions.end(), value, comp_lower);
        assert(result not_eq m_actions.end());

        result->second();

        // find the next entry with a prop > the result's one
        auto const comp_upper = [](auto const prop, auto const& action)
        {
            return prop < action.first;
        };

        auto next = std::upper_bound(result + 1, m_actions.end(), result->first, comp_upper);
        if (next == m_actions.end())
        {
            //no action follows -> erase from here to end
            // - does not change the size of the vector
            // - does not move preceding elements 
            m_performed -= (std::distance(result, next) - 1);
            m_actions.erase(result, next);
            if (m_actions.empty())
            {
                m_sum = 0.0;
            }
            else
            {
                m_sum = m_actions.back().first;
            }
        }
        else
        {
            ++m_performed;
            std::fill(result, next, *next);
        }

    }

    /// reset the action's data (but keep the memory)
    auto clear() -> void
    {
        m_performed = 0;
        m_sum = 0.0;
        m_actions.clear(); // calls the dtors but does not change the capacity
    }


    /// reserve storage for capacity elements
    auto reserve(std::size_t const capacity) -> void
    {
        m_actions.reserve(capacity);
    }

  private:
    /// performed actions
    std::size_t m_performed{0};

    /// accumulated propensities
    double m_sum{0.0};

    /// first: accumulated prop sum, second: the action to perform
    std::vector<std::pair<double, function_type>> m_actions;
};

} // namespace algorithm

#endif

