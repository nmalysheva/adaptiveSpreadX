#include "ContactNetwork.hpp"

#include <utils/Algorithm.hpp>
#include <utils/Json.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>
#include <sstream>


namespace network
{

namespace
{
template <typename FA, typename FB>
auto get_edge_rates(FA get_edge_information, FB get_rate, std::map<ContactNetwork::node_type, Individual> const& population) -> std::vector<EdgeModificationRate>
{
    auto result = std::vector<EdgeModificationRate>{};

    for (auto const& [id, person] : population)
    {
        auto const& edges = get_edge_information(id);
        for (auto const to_id : edges)
        {
            // avoid double listing 
            if (id < to_id)
            {
                auto const rate = get_rate(person, to_id);
                utils::emplace_if_positive(result, rate, id, to_id);
            }
        }
    }
  
    return result;
}
} // namespace


ContactNetwork::ContactNetwork(Settings const& settings)
    : m_species_factory{settings}, m_interaction_manager{settings}
{
    std::for_each(settings.nodes().begin(), settings.nodes().end(), [this] (auto const& node)
    {
        constexpr auto start_time = 0.0;
        utils::repeat_n(node.rate, [this, id = node.identifier] () { create(start_time, id); });
    });

  
    auto get_edges = [this] (auto const id) { return this->m_graph.no_edges_of(id); };
    auto get_rate = [] (auto const& /*unused*/, auto const /*unused*/) { return 1.0; };
    auto const missing_edges = get_edge_rates(get_edges, get_rate, m_population);
    if (missing_edges.empty())
    {
        //sample crashes if empty range
        return;
    }

    auto const count = std::min(missing_edges.size(), settings.edges());
    auto generator = std::default_random_engine{std::random_device{}()};

    auto to_connect = decltype (missing_edges){};
    std::sample(missing_edges.begin(), missing_edges.end(), std::back_inserter(to_connect), count, generator);
    
    std::for_each(to_connect.begin(), to_connect.end(), [this](const auto& edge)
    {
        this->create_edge(edge.from, edge.to);
    });
}



auto ContactNetwork::create(double const simulation_time, State const& state) -> void
{
    auto const new_node = node_type::create();
    auto new_entry = m_species_factory.make(simulation_time, state);
    m_population.emplace(new_node, std::move(new_entry));
    m_graph.add(new_node);
}


auto ContactNetwork::remove(node_type const id) -> void
{
    assert(m_population.count(id) == 1);
    m_population.erase(id);
    m_graph.remove(id);
    m_interaction_manager.remove(id);
}


auto ContactNetwork::get_edge_deletion_rates() const -> std::vector<EdgeModificationRate>
{
    auto get_edges = [this] (auto const id) { return this->m_graph.edges_of(id); };
    auto get_rate = [this] (auto const& person, auto const id) { return person.remove_contact_rate * this->m_population.at(id).remove_contact_rate; };
    
    return get_edge_rates(get_edges, get_rate, m_population);
}


auto ContactNetwork::get_edge_creation_rates() const -> std::vector<EdgeModificationRate>
{
    auto get_edges = [this] (auto const id) { return this->m_graph.no_edges_of(id); };
    auto get_rate = [this] (auto const& person, auto const id) { return person.new_contact_rate * this->m_population.at(id).new_contact_rate; };
    
    return get_edge_rates(get_edges, get_rate, m_population);
}


auto ContactNetwork::get_deaths() const -> std::vector<DeathRate>
{
    auto result = std::vector<DeathRate>{};
    result.reserve(m_population.size());

    for (auto const& [id, individual] : m_population)
    {
        utils::emplace_if_positive(result, individual.death_rate, id);
    }

    return result;
}


auto ContactNetwork::get_births() const -> std::vector<BirthRate>
{
    return m_species_factory.draw_birth_rates();
}
    

auto ContactNetwork::get_transitions() const -> std::vector<TransitionRate>
{
    auto result = std::vector<TransitionRate>{};

    for (auto const& entry : m_population)
    {
        auto const& id = entry.first;
        auto const& rates = entry.second.transition_rates;
        std::transform(rates.begin(), rates.end(), std::back_inserter(result), [id](auto const& entry)
        {
            return TransitionRate{entry.rate, id, entry.identifier};
        });
    }
    
    return result;
}


auto ContactNetwork::get_interactions() const -> std::vector<TransitionRate>
{
    auto result = std::vector<TransitionRate>{};

    auto const& rates = m_interaction_manager.get_rates();

    std::transform(rates.cbegin(), rates.cend(), std::back_inserter(result), [this](auto const& entry)
    {
        auto const& [rate, from, to] = entry;
        auto resulting_state = m_interaction_manager.get_state(m_population.at(from).state, m_population.at(to).state);
        return TransitionRate{rate, from, std::move(resulting_state)};
    });

    return result;
}


auto ContactNetwork::create_edge(node_type const from, node_type const to) -> void
{
    m_graph.connect(from, to);
    m_interaction_manager.add(from, to, m_population.at(from).state, m_population.at(to).state);
}


auto ContactNetwork::delete_edge(node_type const from, node_type const to) -> void
{
    m_graph.disconnect(from, to);
    m_interaction_manager.remove(from, to);
}

 
auto ContactNetwork::change(double const simulation_time, node_type const& id, State const& to_state) -> void
{
    assert(m_population.count(id) == 1);
    m_population.at(id) = m_species_factory.make(simulation_time, to_state);
    m_interaction_manager.remove(id);

    auto const& neighbours = m_graph.edges_of(id);
    for (auto const& neighbour : neighbours)
    {
        m_interaction_manager.add(id, neighbour, to_state, m_population.at(neighbour).state);
    }
}


// LCOV_EXCL_START
auto ContactNetwork::to_json() const -> std::string
{
    auto nodes = utils::json::List<std::string>{};

    auto edge_count = std::size_t{0};

    for (auto const& person : m_population)
    {
        auto block = utils::json::Block{};
        block.add_number("id", static_cast<NodeId::id_type> (person.first));
        block.add_number("last_change", person.second.modification_time);
        block.add_string("state", static_cast<State::value_type> (person.second.state));
        block.add_number("new_contant_rate", person.second.new_contact_rate);
        block.add_number("delete_contant_rate", person.second.remove_contact_rate);
        block.add_number("death_rate", person.second.death_rate);

        auto neighbourhood = utils::json::List<NodeId::id_type>{};

        auto const& neighbours = m_graph.edges_of(person.first);

        for (auto const& neighbour : neighbours)
        {
            neighbourhood.add(neighbour);
        }

        edge_count += neighbours.size();

        block.add_json("neighbours", neighbourhood.to_string());

        auto transitions = utils::json::Block{};
        for (auto const& transition : person.second.transition_rates)
        {
            transitions.add_number(static_cast<State::value_type> (transition.identifier), transition.rate);
        }

        block.add_json("transitions", transitions.to_string());
        
        nodes.add(block.to_string());
    }

    auto json = utils::json::Block{};
    json.add_json("nodes", nodes.to_string());
    json.add_number("node_count", m_population.size());
    json.add_number("edge_count", edge_count);

    return json.to_string();
} 
// LCOV_EXCL_STOP
    

} // namespace network

