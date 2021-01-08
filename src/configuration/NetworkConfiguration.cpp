#include <configuration/NetworkConfiguration.hpp>

#include <utils/String.hpp>

#include <cassert>
#include <stdexcept>
#include <utility>


auto NetworkConfiguration::add(std::string_view line) -> void
{
    assert(not line.empty());
    auto const data = split(line);
   
    constexpr auto EdgeEntries = 1u; 
    constexpr auto NodeEntries = 2u;
    switch (data.size())
    {
        case EdgeEntries:
            if (m_number_of_edges)
            {
                throw std::invalid_argument{"Number of edges must only be set once."};
            }

            m_number_of_edges = to_size_t(data.front());
            break;

        case NodeEntries:
            if (m_data.count(data.front()) == 1)
            {
                throw std::invalid_argument{std::string{"Species must only be set once: "} + data.front()};
            }

            m_data.emplace(std::move(data.front()), to_size_t(data.back()));
            break;

        default:
            throw std::invalid_argument{std::string{"Cannot parse network setting: "} + line.data()};
            break;
    }
}


auto NetworkConfiguration::get_nodes() const noexcept -> std::unordered_map<std::string, std::size_t> const&
{
    return m_data;
}


auto NetworkConfiguration::num_edges() const noexcept -> std::size_t
{
    return m_number_of_edges ? *m_number_of_edges : 0u;
}

