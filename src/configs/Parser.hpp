#ifndef CONFIGS_PARSER_HPP_
#define CONFIGS_PARSER_HPP_

#include <cstddef>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>


/// Parse error alias.
using parse_error = std::runtime_error;


/*!
 * \brief Convert content of string to a desired type.
 *
 * \tparam T target type
 * \param str The string that will be converted.
 * \return Interpretation of string's content as type T.
 */
template <typename T>
[[nodiscard]]
auto to_type(std::string&& str) -> T
{
    if constexpr (std::is_same_v<T, double>)
    {
        return std::stod(str);
    }
    else if constexpr (std::is_same_v<T, std::size_t>)
    {
        return std::stoul(str);
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        return str;
    }
    else
    {
        Template_type_not_supported(new T);
    }
}


template <typename H, typename... TAIL>
[[nodiscard]]
auto do_parse(std::cregex_iterator const it) -> std::tuple<H, TAIL...>
{
    if constexpr (sizeof...(TAIL) == 0)
    {
        auto const ret = to_type<H>(it->str());
        return std::make_tuple(ret);
    }
    else
    {
        return std::tuple_cat(do_parse<H>(it), do_parse<TAIL...>(std::next(it)));
    }
}


template <typename... T>
[[nodiscard]]
auto parse(std::string_view const text) -> std::tuple<T...>
{
    auto const rgx = std::regex{R"((\S+))"};
    auto begin = std::cregex_iterator{text.begin(), text.end(), rgx};
    auto const end = std::cregex_iterator{};

    constexpr auto required_num = sizeof...(T);
    static_assert(required_num > 0, "parse needs at least 1 template parameter");

    auto const given = std::distance(begin, end);
    if (given not_eq required_num)
    {
        std::stringstream msg;
        msg << "parse: wrong number of tokens " << given << " (should be " << required_num << ')';
        throw parse_error{msg.str()};
    }

    return do_parse<T...>(begin);
}

#endif

