#include <network/EdgeInitilisation.hpp>

#include <limits>


EdgeInitilisation::EdgeInitilisation(std::size_t const val) noexcept
: m_count{val}
{
}

auto EdgeInitilisation::exists(EdgeInitilisation const&) const noexcept -> bool
{
    return m_count.has_value();
}


auto EdgeInitilisation::put(std::string const&) noexcept -> bool
{
    return exists(*this);
}

    
auto EdgeInitilisation::count() const noexcept -> std::size_t
{
    return m_count ? *m_count : std::numeric_limits<std::size_t>::max();
}

