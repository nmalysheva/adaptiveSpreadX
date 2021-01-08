#ifndef TYPES_RANDOM_HPP_
#define TYPES_RANDOM_HPP_

/*! \file
 * \author Mathias Lindemann
 *
 * Contains random number generators.
 */

#include <types/Propability.hpp>

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
 * \note For userdefined types (i.e. Propability) min and max must be provided.
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
    assert(not (max < min));

    if constexpr (std::is_floating_point_v<T>)
    {
        static auto generator = std::default_random_engine{std::random_device{}()};
        auto dist = std::uniform_real_distribution<T>(min, max);
        return dist(generator);
    }
    else if constexpr (std::is_integral_v<T>)
    {
        static auto generator = std::default_random_engine{std::random_device{}()};
        auto dist = std::uniform_int_distribution<T>(min, max);
        return dist(generator);
    }
    else if constexpr (std::is_same_v<T, Propability>)
    {
        return Propability{random<>(static_cast<Propability::value_type> (min), static_cast<Propability::value_type> (max))};
    }
    else
    {
        T_is_not_supported(min, max);
    }
}

#endif

