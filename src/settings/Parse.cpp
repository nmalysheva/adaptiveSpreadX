#include "Parse.hpp"

#include <iterator>
#include <regex>


namespace settings::parse
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

auto split_impl(std::string const& str, std::regex const& rgx) -> std::vector<std::string>
{
    auto begin = std::sregex_iterator{str.begin(), str.end(), rgx};
    auto const end = std::sregex_iterator{};

    auto ret = std::vector<std::string>{};
    std::transform(begin, end, std::back_inserter(ret), [](auto const& it)
    {
        return it.str();
    });

    return ret;
}
} // namespace


auto split(std::string const& str) -> std::vector<std::string>
{
    // match only non-whitespace characters
    auto const rgx = std::regex{R"((\S+))"};
    return split_impl(str, rgx);
}


auto to_distribution(std::string const& str) -> Distribution
{
    if (is_double(str))
    {
        return Distribution{std::stod(str)};
    }

    auto const rgx_distribution = std::regex{R"((^[A-Za-z]\((.*)\)$))"}; //check format C(*)
    auto match = std::cmatch{};
    std::regex_match(str.data(), match, rgx_distribution);

    constexpr auto RequiredMatches = 3u; // input, char, (...)
    if (match.size() not_eq RequiredMatches)
    {
        throw std::invalid_argument{NotDistribution};
    }

    // split params at comma
    constexpr auto param_id = 2u;
    auto const params = split_impl(match[param_id], std::regex{R"(([^,]+))"});

    auto params_dbl = std::vector<double>{};
    params_dbl.reserve(params.size());
    std::transform(params.cbegin(), params.cend(), std::back_inserter(params_dbl), to_double);

    return Distribution{str.front(), params_dbl};
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

} // namespace settings::parse

