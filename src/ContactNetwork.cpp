#include "ContactNetwork.hpp"

#include <utils/Random.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>


ContactNetwork::ContactNetwork(NetworkConfiguration const& config, Species const& s)
    : m_species{s}
{
    std::for_each(config.get_nodes().begin(), config.get_nodes().end(), [this](auto const& it)
            {
                for (size_t i = 0u; i < it.second; ++i)
                {
                    this->create(it.first);
                }
            });

    auto const missing_edges = get_edge_creation_rates();
    auto const count = std::min(config.num_edges(), missing_edges.size());
    auto to_connect = decltype (missing_edges){};
    
    auto generator = std::default_random_engine{std::random_device{}()};
    std::sample(missing_edges.begin(), missing_edges.end(), std::back_inserter(to_connect), count, generator);
    
    std::for_each(to_connect.begin(), to_connect.end(), [this](const auto& it)
            {
                auto const [from, to] = it.second;
                this->create_edge(from, to);
            });
}


auto ContactNetwork::create(std::string const& state) -> void
{
    auto const new_node = NodeId::create();
    m_population.emplace(new_node, m_species.create(state));
    m_graph.add(new_node);
}


auto ContactNetwork::remove(NodeId const id) -> void
{
    assert(m_population.count(id) == 1);
    m_population.erase(id);
    m_graph.remove(id);
}

    
auto ContactNetwork::print() const -> void
{
    std::cout << "---------------------\n";
    for (auto const& pop : m_population)
    {
        std::cout << static_cast<NodeId::id_type> (pop.first) << ": " << pop.second.state << '\t';
    }
    std::cout << '\n';
    
    m_graph.print_edges();
    std::cout << "---------------------\n";
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

    
auto ContactNetwork::get_specie(std::string const& name) const -> std::vector<NodeId>
{
    auto ret = std::vector<NodeId>{};

    for (auto const& person_it : m_population)
    {
        if (person_it.second.state == name)
        {
            ret.push_back(person_it.first);
        }
    }

    return ret;
}


auto ContactNetwork::change(NodeId const& id, std::string const& to_state) -> void
{
    assert(m_population.count(id) == 1);
    m_population[id] = m_species.create(to_state);
}


auto ContactNetwork::get_connections(std::string const& from, std::string const& to) const -> std::vector<std::pair<NodeId, NodeId>>
{
    auto ret = std::vector<std::pair<NodeId, NodeId>>{};

    for (auto const& person_it : m_population)
    {
        if (person_it.second.state not_eq from)
        {
            continue;
        }

        auto const& neighbours = m_graph.edges_of(person_it.first);
        for (auto const& neighbour : neighbours)
        {
            assert(m_population.count(neighbour) == 1);
            if (m_population.at(neighbour).state == to)
            {
                ret.emplace_back(person_it.first, neighbour);
            }
        }
    }

    return ret;
}

