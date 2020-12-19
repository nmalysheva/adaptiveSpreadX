#include "ContactNetwork.hpp"

#include <utils/parser.hpp>

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

    
auto ContactNetwork::print() const -> void
{
    /*
    std::cout << "---------------------\n";
    for (auto const& pop : m_population)
    {
        std::cout << pop.first << ": " << pop.second.state << '\t';
    }
    std::cout << '\n';
    
    m_graph.print_edges();
    std::cout << "---------------------\n";
    */
}
    

auto ContactNetwork::get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>
{
    std::vector<std::pair<double, std::pair<NodeId, NodeId>>> vec;
    double sum = 0;

    for (auto const& person_it : m_population)
    {
        auto const id = person_it.first;
        auto const& person = person_it.second;
        auto const& edges = m_graph.edges_of(id);
        for (auto const to_id : edges)
        {
            vec.emplace_back(sum + static_cast<double> (person.new_contact_rate), std::pair<NodeId, NodeId>{id, to_id});
            sum = vec.back().first;
        }
    }

    if (vec.empty())
    {
        return {{0.0, std::pair<NodeId, NodeId>{0xDEADBEEF, 0xDEADBEEF}}};
    }

    return vec;
}


auto ContactNetwork::get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>
{
    std::vector<std::pair<double, std::pair<NodeId, NodeId>>> vec;
    double sum = 0;

    for (auto const& person_it : m_population)
    {
        auto const id = person_it.first;
        auto const& person = person_it.second;
        auto const& edges = m_graph.no_edges_of(id);
        for (auto const to_id : edges)
        {
            vec.emplace_back(sum + static_cast<double> (person.loose_contact_rate), std::pair<NodeId, NodeId>{id, to_id});
            sum = vec.back().first;
        }
    }
  
    if (vec.empty())
    { 
       return {{0.0, std::pair<NodeId, NodeId>{0xDEADBEEF, 0xDEADBEEF}}};
    }

    return vec;
}


auto ContactNetwork::create_edge(NodeId const from, NodeId const to) -> void
{
    m_graph.connect(from, to);
}


auto ContactNetwork::delete_edge(NodeId const from, NodeId const to) -> void
{
    m_graph.disconnect(from, to);
}

