#include "ContactNetwork.hpp"


#include <iostream>

ContactNetwork::ContactNetwork(ConfigurationBlock const& config, Species const& s)
    : m_species{s}, m_graph{0, 0}
{
    auto it = config.begin();
    auto [num_edges] = parse<size_t>(*it);
    ++it;
    auto sum = size_t{0};
    auto id = size_t{0};
    while (it != config.end())
    {
        auto [n, c] = parse<std::string, size_t>(*it);
        for (size_t i = 0; i < c; ++i)
        {
            m_population.emplace(id, m_species.create(n));
            ++id;
        }
        sum += c;
        ++it;
    }

    m_graph = std::move(GraphImpl{sum, num_edges});
}

