#include "GraphImpl.hpp"

#include <cassert>
#include <iterator>
#include <numeric>
#include <random>

#include <algorithm>
#include <vector>
#include <iostream>


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


auto GraphImpl::print_edges() const -> void
{
    std::vector<size_type> vec(m_edges.size());
    std::transform(m_edges.begin(), m_edges.end(), vec.begin(), [](auto const& it)
            {
                return it.first;
            });
    std::sort(vec.begin(), vec.end());

    for (auto const id : vec)
    {
        std::cout << id << ": ";
        auto const& it = m_edges.at(id);
        std::vector<size_type> edges(it.begin(), it.end());
        std::sort(edges.begin(), edges.end());
        for (auto const e : edges)
        {
            std::cout << e << ",";
        }
        std::cout << '\n';
    }
}


auto GraphImpl::edges_of(size_type const node) const -> node_collection_type const&
{
    return m_edges.at(node);
}


auto GraphImpl::no_edges_of(size_type const node) const -> node_collection_type const&
{
    return m_loose.at(node);
}

    
auto GraphImpl::connect(size_type const from, size_type const to) -> void
{
    if (not m_edges.at(from).insert(to).second)
    {
        throw "connect to already set";
    }
    if (not m_loose.at(from).erase(to))
    {
        throw "connect to not available";
    }
}

auto GraphImpl::disconnect(size_type const from, size_type const to) -> void
{
    if (not m_edges.at(from).erase(to))
    {
        throw "disconnect to not available";
    }

    if (not m_loose.at(from).insert(to).second)
    {
        throw "disconnect to already set";
    }
}

