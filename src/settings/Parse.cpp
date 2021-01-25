#include "Parse.hpp"

#include <iterator>
#include <regex>


namespace settings
{
namespace parse
{

namespace
{

auto is_double(std::string const& str) -> bool
{
    auto const rgx_double = std::regex{R"(^\d+(\.\d+)?$)"};
    return std::regex_match(str, rgx_double);
}

auto is_unsigned(std::string const& str) -> bool
{
    auto const rgx_unsigned = std::regex{R"(^\d+$)"};
    return std::regex_match(str, rgx_unsigned);
}

} // namespace


auto split(std::string const& str) -> std::vector<std::string>
{
    auto const rgx = std::regex{R"((\S+))"};
    auto begin = std::sregex_iterator{str.begin(), str.end(), rgx};
    auto const end = std::sregex_iterator{};

    auto ret = std::vector<std::string>{};
    std::transform(begin, end, std::back_inserter(ret), [](auto const& it)
    {
        return it.str();
    });

    return ret;
}


auto to_distribution(std::string const& str) -> Distribution
{
    if (is_double(str))
    {
        return Distribution{std::stod(str)};
    }

    auto const rgx_complex = std::regex{R"((^[A-Za-z]\((.*),(.*)\)$))"};
    auto match = std::cmatch{};
    std::regex_match(str.data(), match, rgx_complex);

    constexpr auto RequiredMatches = 4u; // input, I, dbl, dbl
    if (match.size() not_eq RequiredMatches)
    {
        throw std::invalid_argument{NotDistribution};
    }

    constexpr auto a_id = 2u;
    auto const a = to_double(match[a_id]);
    
    constexpr auto b_id = 3u;
    auto const b = to_double(match[b_id]);

    return Distribution{str.front(), a, b};
}


auto to_double(std::string const& str) -> double
{
    if (not is_double(str))
    {
        throw std::invalid_argument{NotDouble};
    }
    return std::stod(str);
}


auto to_unsigned(std::string const& str) -> std::size_t
{
    if (not is_unsigned(str))
    {
        throw std::invalid_argument{NotUnsigned};
    }

    return static_cast<std::size_t> (std::stoull(str));
}

} // namespace parse
} // namespace settings

