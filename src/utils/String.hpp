#ifndef UTILITIES_STRING_HPP_
#define UTILITIES_STRING_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>

#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
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
 * \param required Number of required tokens
 * \return vector of the single strings
 */
[[nodiscard]]
auto split(std::string_view const str) -> std::vector<std::string>;


template <unsigned R>
[[nodiscard]]
auto split(std::string_view const str)
{
    auto const result = split(str);
    if (result.size() not_eq R)
    {
        auto msg = std::stringstream{};
        msg << result.size() << " entries found, should be " << R;
        throw std::invalid_argument{msg.str()};
    }
    return result;
}

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

/// Parse the line and convert it to std::size_t.
auto to_size_t(std::string_view const str) -> std::size_t;


template <typename T>
auto to_type(std::string_view str) -> T
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        return str.data();
    }
    else if constexpr (std::is_same_v<T, State>)
    {
        return State{str.data()};
    }
    else if constexpr (std::is_same_v<T, std::size_t>)
    {
        return to_size_t(str);
    }
    else if constexpr (std::is_same_v<T, Distribution>)
    {
        return to_distribution(str);
    }
    else
    {
        T::no_conversion_function();
    };
}

template <typename T,
         std::size_t... I>
auto parse(std::string_view str, std::index_sequence<I...>) -> T
{
    constexpr auto Required = std::tuple_size<T>::value;
    auto data = split<Required>(str);
    return std::make_tuple(to_type<typename std::tuple_element<I, T>::type>(data[I])...);
}


template <typename T>
auto parse(std::string_view str) -> std::enable_if_t<std::tuple_size<T>::value not_eq 0, T>
{
    assert(not str.empty());
    return parse<T>(str, std::make_index_sequence<std::tuple_size<T>::value>{});
}

#endif

