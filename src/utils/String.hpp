#ifndef UTILITIES_STRING_HPP_
#define UTILITIES_STRING_HPP_

#include <types/Distribution.hpp>

#include <string>
#include <string_view>
#include <vector>

/*!
 * \brief Remove leading and trailing whitespaces.
 *
 * \param str The string to trim.
 * \return A string containing str without leading and trailing whitespaces.
 */
[[nodiscard]]
auto trim(std::string_view str) -> std::string;

/*!
 * \brief Split a string at whitespaces.
 *
 * \param str The string to split.
 * \return vector of the single strings
 */
[[nodiscard]]
auto split(std::string_view const str) -> std::vector<std::string>;

/*!
 * \brief Create Distribution from string
 *
 * Parses the line and creates a Distribution object.
 * The string has to have one of the formats:
 * - single value -> creates a constant distribution
 * - I(a, b) to create the distribution identified by I with parameters a and b.
 *
 * \see Distribution for more information
 * \throws \see Distribution
 * 
 * \return Distribution according to given configuration.
 */
auto to_distribution(std::string_view const str) -> Distribution;

#endif

