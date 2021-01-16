#include <configuration/Exception.hpp>

namespace configuration
{

Exception::Exception(char const* const msg) noexcept
    : m_msg{msg}
{
}

auto Exception::what() const noexcept -> char const*
{
    return m_msg;
}

} // namespace configuration

