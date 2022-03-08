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
        return m_actions.size();
    }

    /*!
     * \brief Execute first function that has the accumulated value not less than `value`.
     *
     * \note If the last action was performed, sum() will change. Therefore if no new action is added
     * to actions directly a call to sum() is required to make sure the next call is in the valid range.
     *
     * \param value the threshold
     */
    auto call(double const value) -> void
    {
        auto result = find_it(value);
        result->second();
    }


    /*!
     * \brief Same ass `call`, but removes the executed action from the storage.
     *
     * This function can be used for multiple executions of actions while ensuring that each
     * function can only be called once.
     *
     * \note after executing this function a call to `sum()` has to be performed to receive the
     * new maximum value.
     *
     * \param value the threshold
     */
    auto call_and_remove(double const value) -> void
    {
        auto result = find_it(value);
        result->second();

        auto last = std::prev(m_actions.end());
        if (result not_eq last)
        {
            auto const v_prior = (result == m_actions.begin()) ? 0.0 : std::prev(result)->first;
            last->first = last->first - std::prev(last)->first + v_prior;

            auto it = std::prev(last);
            auto const correction = last->first - result->first;
            while (it not_eq result)
            {
                it->first += correction;
                it = std::prev(it);
            }
            
            std::iter_swap(result, last);
        }

        m_actions.pop_back();
        if (m_actions.empty())
        {
            m_sum = 0.0;
        }
        else
        {
            m_sum = m_actions.back().first;
        }
    }

    /// reset the action's data (but keep the memory)
    auto clear() -> void
    {
        m_sum = 0.0;
        m_actions.clear(); // calls the dtors but does not change the capacity
    }


    /// reserve storage for capacity elements
    auto reserve(std::size_t const capacity) -> void
    {
        m_actions.reserve(capacity);
    }

  private:
    /// accumulated propensities
    double m_sum{0.0};

    /// first: accumulated prop sum, second: the action to perform
    std::vector<std::pair<double, function_type>> m_actions;


    /// find the iterator to the action for a given threshold
    [[nodiscard]]
    auto find_it(const double value) -> decltype (m_actions)::iterator
    {
        assert(std::clamp(value, 0.0, m_sum) == value);

        auto const comp_lower = [](auto const& action, auto const prop)
        {
            return action.first < prop;
        };
        auto result = std::lower_bound(m_actions.begin(), m_actions.end(), value, comp_lower);
        assert(result not_eq m_actions.end());
        
        return result;
    }
};

} // namespace algorithm

#endif

