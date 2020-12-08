#include "SimulationConfig.hpp"

#include "Segments.hpp"

#include <stdexcept>
#include <string>
#include <utility>


SimulationConfig::SimulationConfig(std::string_view const line)
{
    auto const segments = Segments{line};
    auto const num_segments = segments.size();

    raise_on_parameter_error(num_segments);
    parse_data(segments.begin());
    raise_on_data_error();
}


auto SimulationConfig::raise(std::string&& msg) const -> void
{
    throw exception_type{std::string{"SimulationConfig: "} + msg};
}


auto SimulationConfig::raise_on_parameter_error(std::ptrdiff_t const given) const -> void
{
    constexpr auto required_entries = 3;

    if (given not_eq required_entries)
    {
        raise(std::string{"wrong number of parameters provided: "} + std::to_string(given) + std::string{" instead of 3"});
    }
}


auto SimulationConfig::parse_data(std::cregex_iterator segment) -> void
{
    m_start = std::stod(segment->str());
    ++segment;
    m_end = std::stod(segment->str());
    ++segment;
    m_steps = std::stoi(segment->str());
}

    
auto SimulationConfig::raise_on_data_error() const -> void
{
    if (m_start > m_end)
    {
        raise("start > end");
    }

    if (m_start == m_end)
    {
        raise("start = end");
    }

    if (m_start < 0 or m_end < 0)
    {
        raise("negative time");
    }

    if (m_steps < 1)
    {
        raise("number of simulations must be > 0");
    }
}

