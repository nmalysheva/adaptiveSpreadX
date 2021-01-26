#include "Stream.hpp"
#include "Exception.hpp"


namespace configuration
{

Stream::Stream(std::istream& input)
    : m_stream{input}
{
    if (not m_stream.good())
    {
        throw Exception{error::NotReadable};
    }
}


auto Stream::has_next_line() -> bool
{
    while (m_stream.good())
    {
        std::string raw;
        std::getline(m_stream, raw);
        
        constexpr auto delim = " \t";
        auto const start = raw.find_first_not_of(delim);
        if (start == std::string::npos)
        {
            continue;
        }
        auto const end = raw.find_last_not_of(delim);
        
        m_current_line = raw.substr(start, 1 + end - start);
        return true;
    }

    m_current_line.clear(); 
    return false;
}

auto Stream::get_line() const noexcept -> std::string const&
{
    return m_current_line;
}

} // namespace configuration

