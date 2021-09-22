#ifndef UTILS_RANDOM_HPP_
#define UTILS_RANDOM_HPP_

namespace utils
{

/*!
 * \brief Draw an random positive double value.
 *
 * Draw a random value in the range (0.0, max].
 *
 * \note The user has to take care, that max is > 0.0.
 *
 * \param max maximum value
 *
 * \return the drawn number
 */
[[nodiscard]]
auto random_double(double max) -> double;

} // namespace utils

#endif

