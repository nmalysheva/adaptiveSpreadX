#include <utils/String.hpp>

#include <cassert>
#include <algorithm>
#include <iterator>
#include <regex>
#include <string>


auto trim(std::string_view str) -> std::string
{
    constexpr auto White_Spaces = " \t";

    auto pos = std::min(str.find_first_not_of(White_Spaces), str.size());
    str.remove_prefix(pos);

    pos = str.find_last_not_of(White_Spaces);
    if (pos not_eq str.npos)
    {
        str.remove_suffix(str.size() - pos - 1);
    }

    return std::string(str.begin(), str.end());
}


auto split(std::string_view const str) -> std::vector<std::string>
{
    auto const rgx = std::regex{R"((\S+))"};
    auto begin = std::cregex_iterator{str.begin(), str.end(), rgx};
    auto const end = std::cregex_iterator{};

    auto const count = std::distance(begin, end);
    auto ret = std::vector<std::string>(count);
    std::transform(begin, end, ret.begin(), [](auto const& it)
            {
                return it.str();
            });
    return ret;
}


auto to_distribution(std::string_view const str) -> Distribution
{
    auto const rgx_double = std::regex{R"((^\d+(\.\d+)?$))"};
    if (std::regex_match(str.data(), rgx_double))
    {
        auto const val = Propability{std::stod(str.data())};
        return Distribution{val};
    }

    auto const rgx_complex = std::regex{R"((^[A-Za-z]\((.*),(.*)\)$))"};
    auto match = std::cmatch{};
    std::regex_match(str.data(), match, rgx_complex);
    
    constexpr auto RequiredMatches = 4u; // input, I, dbl, dbl
    if (match.size() not_eq RequiredMatches)
    {
        throw std::invalid_argument{std::string{"Wrong format: "} + str.data()};
    }

    constexpr auto a_id = 2u;
    constexpr auto b_id = 3u;
    for (auto id : {a_id, b_id})
    {
        if (not std::regex_match(match[id].str(), rgx_double))
        {
            throw std::invalid_argument{std::string{"Invalid parameter: "} + str.data()};
        }
    }

    auto const a = Propability{std::stod(match[a_id].str())};
    auto const b = Propability{std::stod(match[b_id].str())};
    return Distribution{str.front(), a, b};
}


auto to_size_t(std::string_view const str) -> std::size_t
{
    auto const rgx = std::regex{R"((^\d+$))"};
    if (not std::regex_match(str.data(), rgx))
    {
        throw std::invalid_argument{std::string{"Number must be non-negative: "} + str.data()};
    }

    return static_cast<std::size_t> (std::stoull(str.data()));
}

