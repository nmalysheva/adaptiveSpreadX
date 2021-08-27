#include "SSA.hpp"
#include "Actions.hpp"

#include <utils/Random.hpp>

#include <cstddef>
#include <cmath>
#include <functional>


namespace algorithm
{

SSA::SSA(Settings const& settings, network::ContactNetwork& network)
    : m_network{network}, m_rules{settings}
{
}


auto SSA::run(utils::json::Block& json) -> void
{
    auto stats = utils::json::List<std::string>{};
    stats.add(to_json());

    while (execute())
    {
        if (m_step == m_rules.output_step())
        {
            stats.add(to_json());
            m_step = 0;
        }
    }

    if (m_step not_eq 0)
    {
        stats.add(to_json());
    }

    json.add_json("networks", stats.to_string());
}

auto SSA::execute() -> bool
{
    using Nid = network::ContactNetwork::node_type;
    auto m_actions = Actions{};


    auto const del_e = [this] (Nid const a, Nid const b) { this->m_network.delete_edge(a, b); };
    for (auto const& it : m_network.get_edge_deletion_rates())
    {
        if (it.first > 0)
        {
            m_actions.add(it.first, del_e, it.second.first, it.second.second);
        }
    }

    auto const new_e = [this] (Nid const a, Nid const b) { this->m_network.create_edge(a, b); };
    for (auto const& it : m_network.get_edge_creation_rates())
    {
        if (it.first > 0)
        {
            m_actions.add(it.first, new_e, it.second.first, it.second.second);
        }
    }

    auto f_birth = [this] (State const& state) { this->m_network.create(state); };
    for (auto const& birth : m_rules.births())
    {
        auto const r = birth.distribution().draw();
        if (r > 0)
        {
            m_actions.add(r, f_birth, birth.state());
        }
    }


    auto f_death = [this] (State const& state)
    {
        auto const individuals = this->m_network.get_specie(state);
        auto const idx = utils::random<std::size_t>(0, individuals.size() - 1);
        auto const n = *std::next(individuals.cbegin(), idx);
        this->m_network.remove(n); 
    };

    for (auto const& death : m_rules.deaths())
    {
        auto const count = m_network.count_specie(death.state());
        auto const r = death.distribution().draw(count);
        if (r > 0)
        {
            m_actions.add(r, f_death, death.state());
        }
    }


    auto f_trans = [this] (State const& from, State const& to) 
    { 
        auto const individuals = this->m_network.get_specie(from);
        auto const idx = utils::random<std::size_t>(0, individuals.size() - 1);
        auto const n = *std::next(individuals.cbegin(), idx);
        this->m_network.change(n, to); 
    };

    for (auto const& transition : m_rules.transitions())
    {
        auto const count = m_network.count_specie(transition.from());
        auto const r = transition.distribution().draw(count);
        if (r > 0)
        {
            m_actions.add(r, f_trans, transition.from(), transition.to());
        }
    }

    
    auto f_inter = [this] (Nid const n, State const& to) 
    { 
        this->m_network.change(n, to);
    };

    for (auto const& interaction : m_rules.interactions())
    {
        auto const counts = m_network.get_connections(interaction.from(), interaction.connected());
        for (auto const [id, count] : counts)
        {
            auto const r = interaction.distribution().draw(count);
            if (r > 0)
            {
                m_actions.add(r, f_inter, id, interaction.to());
            }
        }
    }


    if (m_actions.sum() == 0.0)
    {
        return false;
    }

    auto r = utils::random<double>();
    auto const proposed_time = 1.0 / m_actions.sum() * std::log(1.0 / r);

    m_now += proposed_time;

    r = utils::random<double>();
    auto const sum_r = m_actions.sum() * r;

    m_actions.call(sum_r);

    // prevent overflow and falsely generated json output
    if (m_step == std::numeric_limits<std::size_t>::max())
    {
        m_step = 1; //LCOV_EXCL_LINE
    }
    else
    {
        ++m_step;
    }

    return m_now <= m_rules.time();
}


auto SSA::to_json() const -> std::string
{
    auto block = utils::json::Block{};

    block.add_number("simulation_time", m_now);
    block.add_json("network", m_network.to_json());
    return block.to_string();
}

} // namespace algorithm

