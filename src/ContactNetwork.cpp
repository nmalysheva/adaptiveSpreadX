#include "ContactNetwork.hpp"

#include <utils/Random.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>


namespace network
{

ContactNetwork::ContactNetwork(Settings const& settings)
    : m_species{std::get<IndividualFactories>(settings)}
{
    auto const& nodes = std::get<NodeInitilisation>(settings).data();
    std::for_each(nodes.begin(), nodes.end(), [this](auto const& it)
            {
                for (size_t i = 0u; i < it.count; ++i)
                {
                    this->create(State{it.state}); //TODO remove ctor
                }
            });

    auto const missing_edges = get_edge_creation_rates();
    auto const edges = std::get<EdgeInitilisation>(settings).count();
    auto const count = std::min(edges, missing_edges.size());
    auto to_connect = decltype (missing_edges){};
    
    auto generator = std::default_random_engine{std::random_device{}()};
    std::sample(missing_edges.begin(), missing_edges.end(), std::back_inserter(to_connect), count, generator);
    
    std::for_each(to_connect.begin(), to_connect.end(), [this](const auto& it)
            {
                auto const [from, to] = it.second;
                this->create_edge(from, to);
            });
}


auto ContactNetwork::create(State const& state) -> void
{
    auto const new_node = NodeId::create();
    auto new_entry = m_species[state].make();
    m_population.emplace(new_node, std::move(new_entry));
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
    /*
    std::cout << "---------------------\n";
    for (auto const& pop : m_population)
    {
        std::cout << static_cast<NodeId::id_type> (pop.first) << ": " << pop.second.state << '\t';
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

    
auto ContactNetwork::get_specie(State const& name) const -> std::vector<NodeId>
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


auto ContactNetwork::change(NodeId const& id, State const& to_state) -> void
{
    assert(m_population.count(id) == 1);
    m_population.at(id) = m_species[to_state].make();
}


auto ContactNetwork::get_connections(State const& from, State const& to) const -> std::vector<std::pair<NodeId, NodeId>>
{
    auto ret = std::vector<std::pair<NodeId, NodeId>>{};

    for (auto const& person_it : m_population)
    {
        if (not (person_it.second.state == from))
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

} // namespace network

