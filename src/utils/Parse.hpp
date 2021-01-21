#ifndef UTILS_PARSE_HPP_
#define UTILS_PARSE_HPP_

#include <types/State.hpp>
#include <types/Distribution.hpp>

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>



namespace parse
{

template <unsigned R>
[[nodiscard]]
auto split(std::string_view const str) 
{
    auto const rgx = std::regex{R"((\S+))"};
    auto begin = std::cregex_iterator{str.begin(), str.end(), rgx};
    auto const end = std::cregex_iterator{};

    auto const count = std::distance(begin, end);
    if (count not_eq R)
    {
        auto msg = std::stringstream{};
        msg << count << " entries found, should be " << R;
        throw std::invalid_argument{msg.str()};
    }

    auto ret = std::array<std::string, R>{};
    std::transform(begin, end, ret.begin(), [](auto const& it)
            {
                return it.str();
            });
    return ret;
}



auto to_unsigned(std::string const& str) -> std::size_t;
auto to_double(std::string const& str) -> double;
auto to_distribution(std::string const& str) -> Distribution;

template <typename T>
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
    else
    {
        T::unknown_type_to_parse();
    }
}

template <typename TPL,
         std::size_t... Is>
auto tuple(std::string const& str, std::index_sequence<Is...>) -> TPL
{
    auto const segments = split<std::tuple_size_v<TPL>>(str);
    return std::make_tuple(to_type<std::tuple_element_t<Is, TPL>>(std::get<Is>(segments))...);
}


template <typename... Ts>
auto to_types(std::string const& str) -> std::tuple<Ts...>
{
    return tuple<std::tuple<Ts...>>(str, std::make_index_sequence<sizeof... (Ts)>{});
}

}
#endif

