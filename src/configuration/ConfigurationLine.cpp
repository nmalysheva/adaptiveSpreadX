#include <configuration/ConfigurationLine.hpp>

#include <utility>


ConfigurationLine::ConfigurationLine(std::string&& line) noexcept
    : m_line{check(std::move(line))}
{
}


auto ConfigurationLine::is_header() const noexcept -> bool
{
    return (not m_line.empty()) and (m_line.front() == '[') and (m_line.back() == ']');
}


auto ConfigurationLine::content() const noexcept -> std::string const&
{
    return m_line;
}


auto ConfigurationLine::check(std::string&& str) noexcept -> std::string
{
    if (str.empty() or str.front() == '#')
    {
        return "";
    }
    return str;
}

