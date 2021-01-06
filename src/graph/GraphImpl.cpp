#include "GraphImpl.hpp"

#include <cassert>
#include <numeric>

#include <algorithm>
#include <vector>
#include <iostream>


auto GraphImpl::add(NodeId const id) -> void
{
    assert(m_loose.count(id) == 0);
    assert(m_edges.count(id) == 0);

    std::for_each(m_loose.begin(), m_loose.end(), [this, id](auto& it)
            {
                this->m_loose[id].insert(it.first);
                it.second.insert(id);
            });
    
    m_loose.emplace(id, std::unordered_set<NodeId>{});
    m_edges.emplace(id, std::unordered_set<NodeId>{});
}


auto GraphImpl::remove(NodeId const id) -> void
{
    assert(m_edges.count(id) == 1);
    m_edges.erase(id);
    std::for_each(m_edges.begin(), m_edges.end(), [id](auto& it) { it.second.erase(id); });

    assert(m_loose.count(id) == 1);
    m_loose.erase(id);
    std::for_each(m_loose.begin(), m_loose.end(), [id](auto& it) { it.second.erase(id); });
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


auto GraphImpl::print_edges() const -> void
{
    std::vector<size_type> vec(m_edges.size());
    std::transform(m_edges.begin(), m_edges.end(), vec.begin(), [](auto const& it)
            {
                return static_cast<NodeId::id_type> (it.first);
            });
    std::sort(vec.begin(), vec.end());

    for (auto const id : vec)
    {
        std::cout << id << ": ";
        auto const& it = m_edges.at(NodeId::refer(id));
        std::vector<size_type> edges(it.begin(), it.end());
        std::sort(edges.begin(), edges.end());
        for (auto const e : edges)
        {
            std::cout << e << ",";
        }
        std::cout << '\n';
    }
}


auto GraphImpl::edges_of(NodeId const node) const -> node_collection_type const&
{
    assert(m_edges.count(node) == 1);
    return m_edges.at(node);
}


auto GraphImpl::no_edges_of(NodeId const node) const -> node_collection_type const&
{
    assert(m_loose.count(node) == 1);
    return m_loose.at(node);
}

    
auto GraphImpl::connect(NodeId const from, NodeId const to) -> void
{
    assert(m_edges.count(from) == 1);
    assert(m_edges[from].count(to) == 0);
    m_edges[from].insert(to);

    assert(m_loose.count(from) == 1);
    assert(m_loose[from].count(to) == 1);
    m_loose[from].erase(to);
}

auto GraphImpl::disconnect(NodeId const from, NodeId const to) -> void
{
    assert(m_edges.count(from) == 1);
    assert(m_edges[from].count(to) == 1);
    m_edges[from].erase(to);

    assert(m_loose.count(from) == 1);
    assert(m_loose[from].count(to) == 0);
    m_loose[from].erase(to);
}

