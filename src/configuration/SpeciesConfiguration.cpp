#include <configuration/SpeciesConfiguration.hpp>

#include <utils/String.hpp>

#include <cassert>
#include <stdexcept>
#include <utility>


auto SpeciesConfiguration::add(std::string_view line) -> void
{
    assert(not line.empty());
    auto const data = split(line);
    
    constexpr auto RequiredEntries = 3u;
    if (data.size() not_eq RequiredEntries)
    {
        throw std::invalid_argument{std::string{"Species needs 3 parameters: "} + line.data()};
    }
    
    m_data.emplace_back(std::move(data[0]), to_distribution(data[1]), to_distribution(data[2]));
}


auto SpeciesConfiguration::get() const noexcept -> std::vector<data_type> const&
{
    return m_data;
}

