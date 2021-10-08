#ifndef UTILS_PARSE_HPP_
#define UTILS_PARSE_HPP_

#include <types/State.hpp>
#include <types/Distribution.hpp>

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>


namespace utils::parse
{

/// string is not a valid non-negative number
constexpr auto NotUnsigned = "string does not match non-negative number";

/// string is not a valid non-negative double
constexpr auto NotDouble = "string does not match non-negative double";

/// string is not a valid distribution
constexpr auto NotDistribution = "string does not match distribution format";

/// split string at whitespaces
[[nodiscard]]
auto split(std::string const& str) -> std::vector<std::string>;

/*!
 * \brief Convert string to unsigned integer.
 *
 * The string must only contain digits.
 *
 * \throw std::invalid_argument string does not fulfil requirements
 * \note std::stoul is used, see docs for further information
 *
 * \param str string to convert
 * \return numeric value of str
 */
[[nodiscard]]
auto to_unsigned(std::string const& str) -> std::size_t;


/*!
 * \brief Convert string to non-negative double.
 *
 * The string must only contain digits and optionally a dot.
 *
 * \throw std::invalid_argument string does not fulfil requriements
 * \note std::stod is used, see docs for further information
 *
 * \param str string to convert
 * \return double value of str
 */
[[nodiscard]]
auto to_double(std::string const& str) -> double;


/*!
 * \brief Convert string to a Distribution.
 *
 * If the input is a double (\see to_double) a fixed Distribution is returned.
 * Otherwise it must have the format `X(p0,...)`. Where `X` denotes the type of
 * distribution and `ps` (all doubles) are the parameters of the distribution.
 *
 * \throw std::invalid_argument string does not fulfil requirements
 * \see Distribution for more information
 *
 * \param str string to convert
 * \return Distribution with parameters of str
 */
[[nodiscard]]
auto to_distribution(std::string const& str) -> Distribution;

/*!
 * \brief Convert a string to a given type.
 *
 * Convert a string to a desired type.
 * Currently only `std::size_t`, `double`, `State`, and `Distribution` are supported.
 *
 * \tparam T desired type
 * \param str string to convert
 * \return str converted to value of type T
 */
template <typename T>
[[nodiscard]]
auto to_type(std::string const& str) -> T
{
    using C = std::remove_cv_t<T>;

    if constexpr (std::is_same_v<C, std::size_t>)
    {
        return to_unsigned(str);
    }
    else if constexpr (std::is_same_v<C, double>)
    {
        return to_double(str);
    }
    else if constexpr (std::is_same_v<C, State>)
    {
        return State{str};
    }
    else if constexpr (std::is_same_v<C, Distribution>)
    {
        return to_distribution(str);
    }
    else if constexpr (std::is_same_v<C, std::string>)
    {
        return str;
    }
    else
    {
        T::unknown_type_to_parse();
    }
}


/*!
 * \brief Convert a string to a tuple of types.
 *
 * \throws std::invalid_argument if string does not contain proper number of segments
 *
 * \tparam TPL tuple of desired types
 * \tparam Is integer sequence for accessing elements of TPL
 * \param str string to convert
 * \return tuple with string parsed to given types
 */
template <typename TPL,
         std::size_t... Is>
[[nodiscard]]
auto tuple(std::string const& str, std::index_sequence<Is...> /*unused*/) -> TPL
{
    auto const segments = split(str);

    if (segments.size() not_eq std::tuple_size_v<TPL>)
    {
        auto msg = std::stringstream{};
        msg << segments.size() << " entries found, should be " << std::tuple_size_v<TPL>;
        throw std::invalid_argument{msg.str()};
    }

    return std::make_tuple(to_type<std::tuple_element_t<Is, TPL>>(segments[Is])...);
}


/*!
 * \brief Parse a string to given types.
 *
 * This is just a convenience function for \ref tuple.
 * \attention see tuple for information!
 *
 * \tparam Ts desired types 
 * \param str string to conver
 * \return `std::tuple` containing the parsed values
 */
template <typename... Ts>
[[nodiscard]]
auto to_types(std::string const& str) -> std::tuple<Ts...>
{
    return tuple<std::tuple<Ts...>>(str, std::make_index_sequence<sizeof... (Ts)>{});
}

} // namespace settings::parse

#endif

