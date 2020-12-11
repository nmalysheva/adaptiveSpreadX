#include "GraphImpl.hpp"

#include <cassert>
#include <iterator>
#include <numeric>
#include <random>


GraphImpl::GraphImpl(size_type const nodes, size_type const edges)
{
    if (nodes == 0)
    {
        return;
    }

    // max (nodes over 2) edges possible
    // (n * (n - 1)) / 2 >= edges
    // (n - 1) / 2 =>  (edges / n)
    auto const max_edges = (nodes - 1) / 2;
    auto const wanted = edges / nodes;
    if (max_edges < wanted)
    {
        raise("too many edges");
    }

    for (size_type i = 0; i < nodes; ++i)
    {
        m_edges.emplace(i, std::unordered_set<size_type>{});

        for (size_type j = 0; j < nodes; ++j)
        {
            if (j != i)
            {
                m_loose[i].insert(j);
            }
        }
    }


    auto generator = std::mt19937{std::random_device{}()};
    for (size_type k = 0; k < edges; ++k)
    {
        //draw from
        auto dist_f = std::uniform_int_distribution<size_type>{0, m_loose.size() - 1};
        auto const from = dist_f(generator);
        auto const from_it = std::next(m_loose.begin(), from);


        //draw to
        auto dist_t = std::uniform_int_distribution<size_type>{0, from_it->second.size() - 1};
        auto const to = dist_t(generator);
        auto to_it = std::next(from_it->second.begin(), to);

        //transfer node
        m_edges[from_it->first].insert(m_loose[from_it->first].extract(to_it));

        //if node is fully connected: remove from list
        if (m_loose[from_it->first].empty())
        {
            m_loose.erase(from_it->first);
        }
    }
}


auto GraphImpl::num_nodes() const -> size_type
{
    return m_edges.size();
}


auto GraphImpl::num_edges() const -> size_type
{
    constexpr auto init = size_type{0};
    return std::accumulate(m_edges.cbegin(), m_edges.cend(), init, [](auto n, auto const& it)
            {
            return n + it.second.size();
            });
}


auto GraphImpl::raise(std::string&& msg) -> void
{
    throw exception_type{std::string{"GraphImpl: "} + msg};
}

