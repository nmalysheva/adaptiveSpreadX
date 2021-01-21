#include "GraphImpl.hpp"

#include <cassert>
#include <algorithm>
#include <vector>


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
    m_edges[to].insert(from);

    assert(m_loose.count(from) == 1);
    assert(m_loose[from].count(to) == 1);
    m_loose[from].erase(to);
    m_loose[to].erase(from);
}


auto GraphImpl::disconnect(NodeId const from, NodeId const to) -> void
{
    assert(m_edges.count(from) == 1);
    assert(m_edges[from].count(to) == 1);
    m_edges[from].erase(to);
    m_edges[to].erase(from);

    assert(m_loose.count(from) == 1);
    assert(m_loose[from].count(to) == 0);
    m_loose[from].insert(to);
    m_loose[to].insert(from);
}

