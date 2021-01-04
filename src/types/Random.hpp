#ifndef TYPES_RANDOM_HPP_
#define TYPES_RANDOM_HPP_

/*! \file
 * \author Mathias Lindemann
 *
 * Contains random number generators.
 */

#include <cassert>
#include <limits>
#include <random>
#include <type_traits>


/*!
 * \brief Draw random value.
 *
 * Draw a random value in the range [min, max].
 *
 * Defaults are:
 * - min = std::numeric_limits<T>::min()
 * - max = T{1}
 *
 * \tparam T The type to draw.
 * \param min Lower bound
 * \param max Upper bound
 *
 * \return The drawn number.
 */
template <typename T>
auto random(T const min = std::numeric_limits<T>::min(), T const max = T{1}) noexcept -> T
{
    assert(min <= max);

    static auto generator = std::default_random_engine{std::random_device{}()};
    
    if constexpr (std::is_floating_point_v<T>)
    {
        auto dist = std::uniform_real_distribution<T>(min, max);
        return dist(generator);
    }
    else if constexpr (std::is_integral_v<T>)
    {
        auto dist = std::uniform_int_distribution<T>(min, max);
        return dist(generator);
    }
    else
    {
        T_is_not_supported_for_random_numbers(min, max);
    }
}

#endif

