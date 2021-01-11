#include <configuration/TransitionConfiguration.hpp>

#include <utils/String.hpp>

#include <cassert>
#include <stdexcept>


auto TransitionConfiguration::add(std::string_view line) -> void
{
    assert(not line.empty());
    constexpr auto RequiredEntries = 4u;
    auto const data = split<RequiredEntries>(line);

    if (data[1] not_eq "->")
    {
        throw std::invalid_argument{"wrong format"};
    }

    auto it = m_data.find(data.front());
    if (it == m_data.end())
    {
        it = m_data.emplace(std::move(data.front()), DataType{}).first;
    }
    else
    {
        if (it->second.count(data[2]) not_eq 0)
        {
            throw std::invalid_argument{"rule already set"};
        }
    }

    it->second.emplace(std::move(data[2]), to_distribution(data[3]));
}


auto TransitionConfiguration::get() const noexcept -> std::unordered_map<std::string, DataType> const&
{
    return m_data;
}

