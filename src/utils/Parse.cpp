#include <utils/Parse.hpp>

#include <types/Distribution.hpp>
#include <types/State.hpp> 
#include <iterator>
#include <regex>


namespace
{
auto is_double(std::string_view const str) -> bool
{
    auto const rgx_double = std::regex{R"(^\d+(\.\d+)?$)"};
    return std::regex_match(str.data(), rgx_double);
}

} // namespace


namespace parse
{
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
        throw std::invalid_argument{std::string{"Wrong format: "} + str.data()};
    }

    constexpr auto a_id = 2u;
    constexpr auto b_id = 3u;

    auto const a = to_double(match[a_id]);
    auto const b = to_double(match[b_id]);
    return Distribution{str.front(), a, b};
}

auto to_double(std::string const& str) -> double
{
    if (not is_double(str))
    {
        throw "wrong format for double";
    }
    return std::stod(str);
}

auto to_unsigned(std::string const& str) -> std::size_t
{
    auto const rgx = std::regex{R"(^\d+$)"};
    if (not std::regex_match(str, rgx))
    {
        throw std::invalid_argument{std::string{"Number must be non-negative: "} + str};
    }

    return static_cast<std::size_t> (std::stoull(str));
}
}
