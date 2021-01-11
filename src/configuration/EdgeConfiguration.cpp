#include <configuration/EdgeConfiguration.hpp>

#include <utils/String.hpp>

#include <cassert>
#include <limits>
#include <stdexcept>


auto EdgeConfiguration::add(std::string_view line) -> void
{
    assert(not line.empty());

    if (m_data)
    {
        throw std::invalid_argument{"number of edges already set"};
    }

    constexpr auto RequiredEntries = 1u;
    auto const data = split<RequiredEntries>(line);
    m_data = to_size_t(data.front());
}


auto EdgeConfiguration::get() const noexcept -> std::size_t
{
    return m_data ? *m_data : std::numeric_limits<std::size_t>::max();
}

