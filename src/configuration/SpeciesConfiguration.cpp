#include <configuration/SpeciesConfiguration.hpp>

#include <utils/String.hpp>

#include <cassert>
#include <stdexcept>


auto SpeciesConfiguration::add(std::string_view line) -> void
{
    assert(not line.empty());
    auto const data = split(line);
    
    constexpr auto RequiredEntries = 3u;
    if (data.size() not_eq RequiredEntries)
    {
        throw std::invalid_argument{std::string{"Species needs 3 parameters: "} + line.data()};
    }
    
    if (m_data.count(data.front()) == 1)
    {
        throw std::invalid_argument{std::string{"Species already set: "} + data.front()};
    } 

    m_data.emplace(std::move(data.front()), std::make_pair(to_distribution(data[1]), to_distribution(data[2])));
}


auto SpeciesConfiguration::get() const noexcept -> std::unordered_map<std::string, DistributionPair> const&
{
    return m_data;
}

