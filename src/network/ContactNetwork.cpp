#include "ContactNetwork.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>


namespace network
{

namespace
{
template <typename FA, typename FB>
auto get_edge_rates(FA get_edge_information, FB get_rate, std::map<ContactNetwork::node_type, Individual> const& population) -> std::vector<std::pair<double, std::pair<ContactNetwork::node_type, ContactNetwork::node_type>>>
{
    std::vector<std::pair<double, std::pair<ContactNetwork::node_type, ContactNetwork::node_type>>> vec;

    for (auto const& [id, person] : population)
    {
        auto const& edges = get_edge_information(id);
        for (auto const to_id : edges)
        {
            // avoid double listing 
            if (id < to_id)
            {
                auto const rate = get_rate(person, to_id);
                vec.emplace_back(rate, std::make_pair(id, to_id));
            }
        }
    }
  
    return vec;
}
} // namespace

ContactNetwork::ContactNetwork(Settings const& settings)
    : m_species{settings.factory()}
{
    for (auto const& state : settings.states())
    {
        m_detailed_population.emplace(state, std::set<node_type>{});
    }

    for (auto const& node : settings.nodes())
    {
        for (std::size_t i = 0u; i < node.second; ++i)
        {
            this->create(node.first);
        }
    }

    auto const missing_edges = get_edge_creation_rates();
    if (missing_edges.empty())
    {
        //sample crashes if empty range
        return;
    }

    auto const count = std::min(missing_edges.size(), settings.edges());
    auto generator = std::default_random_engine{std::random_device{}()};

    auto to_connect = decltype (missing_edges){};
    std::sample(missing_edges.begin(), missing_edges.end(), std::back_inserter(to_connect), count, generator);
    
    std::for_each(to_connect.begin(), to_connect.end(), [this](const auto& it)
            {
                auto const [from, to] = it.second;
                this->create_edge(from, to);
            });
    
}



auto ContactNetwork::create(State const& state) -> void
{
    auto const new_node = node_type::create();
    auto new_entry = m_species.make(state);
    m_detailed_population[new_entry.state()].insert(new_node);
    m_population.emplace(new_node, std::move(new_entry));
    m_graph.add(new_node);
}


auto ContactNetwork::remove(node_type const id) -> void
{
    assert(m_population.count(id) == 1);
    m_detailed_population[m_population.at(id).state()].erase(id);
    m_population.erase(id);
    m_graph.remove(id);
}


auto ContactNetwork::get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<node_type, node_type>>>
{
    auto get_edges = [this] (auto const id) { return this->m_graph.edges_of(id); };
    auto get_rate = [this] (auto const& person, auto const id) { return std::max(person.remove_contact_rate(), this->m_population.at(id).remove_contact_rate()); };
    
    return get_edge_rates(get_edges, get_rate, m_population);
}


auto ContactNetwork::get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<node_type, node_type>>>
{
    auto get_edges = [this] (auto const id) { return this->m_graph.no_edges_of(id); };
    auto get_rate = [this] (auto const& person, auto const id) { return std::min(person.new_contact_rate(), this->m_population.at(id).new_contact_rate()); };
    
    return get_edge_rates(get_edges, get_rate, m_population);
}



auto ContactNetwork::create_edge(node_type const from, node_type const to) -> void
{
    m_graph.connect(from, to);
}


auto ContactNetwork::delete_edge(node_type const from, node_type const to) -> void
{
    m_graph.disconnect(from, to);
}

 
auto ContactNetwork::get_specie(State const& name) const -> std::vector<node_type>
{
    auto ret = std::vector<node_type>{};
    assert(m_detailed_population.count(name) == 1);

    auto const& entries = m_detailed_population.at(name);
    ret.reserve(entries.size());
    std::copy(entries.cbegin(), entries.cend(), std::back_inserter(ret));
    
    return ret;
}


auto ContactNetwork::count_specie(State const& name) const -> std::size_t
{
    assert(m_detailed_population.count(name) == 1);
    return m_detailed_population.at(name).size();
}


auto ContactNetwork::change(node_type const& id, State const& to_state) -> void
{
    assert(m_population.count(id) == 1);

    m_detailed_population.at(m_population.at(id).state()).erase(id);
    m_population.at(id) = m_species.make(to_state);
    m_detailed_population.at(m_population.at(id).state()).insert(id);
}


auto ContactNetwork::get_connections(State const& from, State const& to) const -> std::vector<std::pair<node_type, std::size_t>>
{
    auto ret = std::vector<std::pair<node_type, std::size_t>>{};
    assert(m_detailed_population.count(from) == 1);
    
    auto const same_states = from == to;

    for (auto const& person : m_detailed_population.at(from))
    {
        auto const& neighbours = m_graph.edges_of(person);

        auto const count = std::count_if(neighbours.cbegin(), neighbours.cend(), [same_states, person, &to, this] (auto const neighbour)
        {
            if (same_states and not (person < neighbour))
            {
                return false;
            }
            return this->m_population.at(neighbour).state() == to;
        });
        
        if (count not_eq 0)
        {
            ret.emplace_back(person, count);
        }
    }

    return ret;
}

} // namespace network

