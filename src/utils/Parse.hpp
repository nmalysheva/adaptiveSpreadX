#ifndef UTILS_PARSE_HPP_
#define UTILS_PARSE_HPP_

#include <algorithm>
#include <stdexcept>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>


namespace parse
{

template <typename A,
          std::size_t... Is>
auto array_to_tuple(A const& array, std::index_sequence<Is...>)
{
    return std::make_tuple(std::get<Is>(array)...);
}

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

    return array_to_tuple(ret, std::make_index_sequence<R>{});
}

} // namespace parse

#endif

