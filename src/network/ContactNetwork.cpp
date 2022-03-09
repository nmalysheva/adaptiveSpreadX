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
auto get_edge_rates(std::vector<EdgeModificationRate>& container, FA get_edge_information, FB get_rate, std::map<ContactNetwork::node_type, Individual> const& population) -> void
{
    container.clear();

    for (auto const& [id, person] : population)
    {
        auto const& edges = get_edge_information(id);
        for (auto const to_id : edges)
        {
            // avoid double listing 
            if (id < to_id)
            {
                auto const rate = get_rate(person, to_id);
                utils::emplace_if_positive(container, rate, id, to_id);
            }
        }
    }
}
} // namespace


ContactNetwork::ContactNetwork(Settings const& settings)
    : m_species_factory{settings},
      m_interaction_manager{settings},
      m_adaptions{settings.adaptions()}
{
    for (auto const& state : settings.states())
    {
        m_state_db.emplace(state, 0);
    }

    std::for_each(settings.nodes().begin(), settings.nodes().end(), [this] (auto const& node)
    {
        constexpr auto start_time = 0.0;
        utils::repeat_n(node.second, [this, id = node.first] () { create(start_time, id); });
    });


    if (settings.edges() == 0)
    {
        return;
    }
 
    // randomly create edges (use m_edge_create_rates and m_edge_remove_rates to save some memory allocations) 
    auto get_edges = [this] (auto const id) { return this->m_graph.no_edges_of(id); };
    auto get_rate = [] (auto const& /*unused*/, auto const /*unused*/) { return 1.0; };
    get_edge_rates(m_edge_create_rates, get_edges, get_rate, m_population);
    if (m_edge_create_rates.empty())
    {
        //sample crashes if empty range
        return; //LCOV_EXCL_LINE
    }

    auto const count = std::min(m_edge_create_rates.size(), settings.edges());
    auto generator = std::default_random_engine{settings.seed()};

    m_edge_remove_rates.reserve(count);
    std::sample(m_edge_create_rates.begin(), m_edge_create_rates.end(), std::back_inserter(m_edge_remove_rates), count, generator);
    
    std::for_each(m_edge_remove_rates.begin(), m_edge_remove_rates.end(), [this](const auto& edge)
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
    ++m_state_db[state];
}


auto ContactNetwork::remove(node_type const id) -> void
{
    assert(m_population.count(id) == 1);
    --m_state_db[m_population.at(id).state];
    m_population.erase(id);
    m_graph.remove(id);
    m_interaction_manager.remove(id);
}


auto ContactNetwork::get_edge_deletion_rates()  -> std::vector<EdgeModificationRate> const&
{
    auto get_edges = [this] (auto const id) { return this->m_graph.edges_of(id); };
    auto get_rate = [this] (auto const& person, auto const id) { return person.remove_contact_rate * this->m_population.at(id).remove_contact_rate; };
    
    get_edge_rates(m_edge_remove_rates, get_edges, get_rate, m_population);
    return m_edge_remove_rates;
}


auto ContactNetwork::get_edge_creation_rates() -> std::vector<EdgeModificationRate> const&
{
    auto get_edges = [this] (auto const id) { return this->m_graph.no_edges_of(id); };
    auto get_rate = [this] (auto const& person, auto const id) { return person.new_contact_rate * this->m_population.at(id).new_contact_rate; };
    
    get_edge_rates(m_edge_create_rates, get_edges, get_rate, m_population);
    return m_edge_create_rates;
}


auto ContactNetwork::get_deaths() -> std::vector<DeathRate> const&
{
    m_death_rates.clear();
    m_death_rates.reserve(m_population.size());

    for (auto const& [id, individual] : m_population)
    {
        utils::emplace_if_positive(m_death_rates, individual.death_rate, id);
    }

    return m_death_rates;
}


auto ContactNetwork::get_births() const -> std::vector<BirthRate> const&
{
    return m_species_factory.draw_birth_rates();
}
    

auto ContactNetwork::get_transitions() -> std::vector<TransitionRate> const&
{
    m_transition_rates.clear();
    m_transition_rates.reserve(m_population.size());

    for (auto const& entry : m_population)
    {
        auto const& id = entry.first;
        auto const& rates = entry.second.transition_rates;
        std::transform(rates.begin(), rates.end(), std::back_inserter(m_transition_rates), [id](auto const& entry)
        {
            return TransitionRate{entry.rate, id, entry.identifier};
        });
    }
    
    return m_transition_rates;
}


auto ContactNetwork::get_interactions() -> std::vector<TransitionRate> const&
{
    auto const& rates = m_interaction_manager.get_rates();
    m_interaction_rates.clear();
    m_interaction_rates.reserve(rates.size());

    std::transform(rates.cbegin(), rates.cend(), std::back_inserter(m_interaction_rates), [this](auto const& entry)
    {
        auto const& [rate, from, to] = entry;
        auto resulting_state = m_interaction_manager.get_state(m_population.at(from).state, m_population.at(to).state);
        return TransitionRate{rate, from, std::move(resulting_state)};
    });

    return m_interaction_rates;
}


auto ContactNetwork::create_edge(node_type const from, node_type const to) -> void
{
    m_graph.connect(from, to);
    m_interaction_manager.add(from, to, m_population.at(from).state, m_population.at(to).state);

    auto const rate = m_population.at(from).remove_contact_rate * m_population.at(to).remove_contact_rate;
    m_last_egdge_undo = EdgeModificationRate{rate, from, to};
}


auto ContactNetwork::delete_edge(node_type const from, node_type const to) -> void
{
    m_graph.disconnect(from, to);
    m_interaction_manager.remove(from, to);
    
    auto const rate = m_population.at(from).new_contact_rate * m_population.at(to).new_contact_rate;
    m_last_egdge_undo = EdgeModificationRate{rate, from, to};
}

 
auto ContactNetwork::change(double const simulation_time, node_type id, State to_state) -> void
{
    auto todos = std::list<std::pair<node_type, State>>{std::make_pair(id, std::move(to_state))};

    while (not todos.empty())
    {

        id = todos.front().first;
        to_state = std::move(todos.front().second);
        todos.pop_front();

        auto const& neighbours = m_graph.edges_of(id);
        if (neighbours.empty())
        {
            // no neighbours -> no adaptions will happen
            change_state(simulation_time, id, to_state);
            continue;
        }

        auto adaption_it = m_adaptions.find(to_state);
        if (adaption_it == m_adaptions.end())
        {
            // no adaptions
            change_state(simulation_time, id, to_state);
            continue;
        }
            
        assert(not adaption_it->second.empty());

        // select a SINGLE random adaption
        auto generator = std::default_random_engine{std::random_device{}()};
        constexpr auto single = 1;
        auto adaption_arr = std::array<AdaptionData, single>{};
        auto const& adaption = adaption_arr.front();

        std::sample(adaption_it->second.begin(), adaption_it->second.end(), adaption_arr.begin(), single, generator);
        auto const count = static_cast<std::size_t> (std::round(neighbours.size() * adaption.percentage));
        if (count == 0)
        {
            // if 0% of the neighbours / edges are affected: skip it
            change_state(simulation_time, id, to_state);
            continue;
        }

        auto selected_neighbours = std::vector<node_type>{};
        selected_neighbours.reserve(neighbours.size());
        auto sn_bi = std::back_inserter(selected_neighbours);

        if (not adaption.result.has_value()) // handle edge removal
        {
            std::sample(neighbours.begin(), neighbours.end(), sn_bi, count, generator);
            for (auto const neighbour : selected_neighbours)
            {
                delete_edge(id, neighbour);
            }
        }
        else // handle neighbour changes
        {
            if (adaption.who.empty()) // take all neighbours into account
            {
                std::copy_if(neighbours.begin(), neighbours.end(), sn_bi, [this,&adaption](auto const n)
                {
                    return not (this->m_population.at(n).state == adaption.result);
                });
            }
            else // only with matching type
            {
                std::copy_if(neighbours.begin(), neighbours.end(), sn_bi, [this, &adaption](auto const n)
                {
                    return std::binary_search(adaption.who.begin(), adaption.who.end(), this->m_population.at(n).state);
                });
            }

            auto const selected_count = static_cast<std::size_t> (std::round(selected_neighbours.size() * adaption.percentage));
            if (selected_count not_eq 0)
            {
                auto final_selection = std::vector<node_type>{};
                final_selection.reserve(selected_count);
                std::sample(selected_neighbours.begin(), selected_neighbours.end(), std::back_inserter(final_selection), selected_count, generator);
                for (auto const n : final_selection)
                {
                    todos.emplace_back(n, adaption.result.value());
                }
            }
        }
        
        change_state(simulation_time, id, to_state);
    }
}
    

auto ContactNetwork::change_state(double const simulation_time, node_type const& id, State const& to_state) -> void
{
    assert(m_population.count(id) == 1);
    auto& individual = m_population.at(id);
    --m_state_db[individual.state];
    individual = m_species_factory.make(simulation_time, to_state);
    ++m_state_db[to_state];

    m_interaction_manager.remove(id);
    auto const& neighbours = m_graph.edges_of(id);
    for (auto const& neighbour : neighbours)
    {
        m_interaction_manager.add(id, neighbour, to_state, m_population.at(neighbour).state);
    }
}

auto ContactNetwork::get_state_counts() const noexcept -> std::map<State, std::size_t> const&
{
    return m_state_db;
}

    
auto ContactNetwork::get_max_interaction_rates() const noexcept -> std::vector<StateTransitionRate>
{
    return m_interaction_manager.max_rates();
}
    

auto ContactNetwork::get_last_edge_undo() const -> EdgeModificationRate
{
    assert(m_last_egdge_undo);
    return m_last_egdge_undo.value();
}

// LCOV_EXCL_START
auto ContactNetwork::to_json() const -> std::string
{
    auto const& inter_rates = m_interaction_manager.get_rates();
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
        

        auto inter_json = utils::json::List<std::string>{};
        for (auto const& [rate, from, to] : inter_rates)
        {
            if (from == person.first)
            {
                auto entry = utils::json::Block{};
                entry.add_number("id", static_cast<NodeId::id_type> (to));
                entry.add_number("rate", rate);
                auto const& resulting_state = m_interaction_manager.get_state(person.second.state,m_population.at(to).state);
                entry.add_string("state", static_cast<State::value_type> (resulting_state));
                inter_json.add(entry.to_string());
            }
        }
        block.add_json("interactions", inter_json.to_string());
        
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

