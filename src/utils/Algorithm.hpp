#ifndef UTILS_ALGORITHM_HPP_
#define UTILS_ALGORITHM_HPP_

#include <utility>


namespace utils
{

/*!
 * \brief Call emplace_back on a container, iff the first parameter of emplace is positive
 *
 * If an event has a propensity rate of 0, it will not happen.
 * To avoid storing it in a container, this function can be used to omit the emplacement.
 *
 * \tparam C the type of container to use
 * \tparam T the datatype of the rate (must be comparable to 0.0)
 * \tparam Ts the remaining data
 *
 * \param cont the container on which `emplace_back` will be called
 * \param t the rate
 * \tparam vals the other values that will be stored
 */
template <typename C,
          typename T,
          typename... Ts>
auto emplace_if_positive(C& cont, T const t, Ts&&... vals) -> void
{
    if (t > 0.0)
    {
        cont.emplace_back(t, std::forward<Ts&&>(vals)...);
    }
}


/*!
 * \brief Helper function to avoid `for` and `while` if the function to call does not depend on the counter.
 *
 * \tparam F type of parameter fun, which must be a callable
 *
 * \param n number of calls
 * \param fun the function to execute
 */
template <typename F>
auto repeat_n(std::size_t n, F fun) -> void
{
    while (n > 0)
    {
        fun();
        --n;
    }
}

}

#endif

