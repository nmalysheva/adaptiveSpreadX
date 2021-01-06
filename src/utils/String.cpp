#include <utils/String.hpp>

#include <algorithm>


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

