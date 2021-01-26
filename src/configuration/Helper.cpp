#include "Helper.hpp"

#include "Exception.hpp"

#include <cassert>


namespace configuration::helper
{

auto classify(std::string const& str) noexcept -> Category
{
    if (str.empty())
    {
        return Category::eos;
    }

    switch (str.front())
    {
        case marker::Comment:
            return Category::comment;

        case marker::HeaderStart:
            if (str.back() == marker::HeaderEnd)
            {
                return Category::header;
            }
            [[fallthrough]];

        default:
            return Category::data;
    }
}
    

auto find_first_header(Stream& stream) -> bool
{
    while (stream.has_next_line())
    {
        auto const& line = stream.get_line();
        switch (helper::classify(line))
        {
            case Category::data:
                throw Exception{error::NoHeader};

            case Category::header:

                return true;
            default:
                break;
        }
    }

    return false;
}


auto unheader(std::string const& str) -> std::string
{
    assert(classify(str) == Category::header);
    constexpr auto EmptyHeaderSize = 2u;
    if (str.size() == EmptyHeaderSize)
    {
        throw Exception{error::EmptyHeader};
    }
    return std::string(std::next(str.begin()), std::prev(str.end()));
}

} // namespace configuration::helper

