#ifndef UTILS_RANDOM_HPP_
#define UTILS_RANDOM_HPP_

#include <cassert>
#include <limits>
#include <random>
#include <type_traits>


namespace utils
{

/*!
 * \brief Draw random value.
 *
 * Draw a random value in the range [min, max].
 *
 * Defaults are:
 * - min = std::numeric_limits<T>::min()
 * - max = T{1}
 *
 * \note For userdefined types min and max must be provided.
 *
 * \tparam T the type to draw
 * \param min minimum value
 * \param max maximum value
 *
 * \return the drawn number
 */
template <typename T>
auto random(T const min = std::numeric_limits<T>::min(), T const max = T{1}) -> T
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
    else
    {
        T_is_not_supported(min, max);
    }
}

} // namespace utils

#endif

