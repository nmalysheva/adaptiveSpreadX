#ifndef UTILITIES_STRING_HPP_
#define UTILITIES_STRING_HPP_

#include <string>
#include <string_view>

/*!
 * \brief Remove leading and trailing whitespaces.
 *
 * \param str The string to trim.
 * \return A string containing str without leading and trailing whitespaces.
 */
auto trim(std::string_view str) -> std::string;

#endif

