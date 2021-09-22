#include "SSA.hpp"

#include <utils/Random.hpp>

#include <array>
#include <cstddef>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>


namespace algorithm
{

SSA::SSA(Settings const& settings, network::ContactNetwork& network)
    : m_network{network}, m_settings{settings}
{
}


auto SSA::run(utils::json::Block& json) -> void
{
    auto stats = utils::json::List<std::string>{};
    stats.add(to_json());

    while (execute())
    {
        // LCOV_EXCL_START
        if (m_step == m_settings.output_step())
        {
            stats.add(to_json());
            m_step = 0;
        }
        // LCOV_EXCL_STOP
    }

    if (m_step not_eq 0)
    {
        stats.add(to_json());
    }

    json.add_json("networks", stats.to_string());
}


auto SSA::execute() -> bool
{
    auto actions = std::array
    {
        get_delete_edge_actions(),
        get_create_edge_actions(),
        get_birth_actions(),
        get_death_actions(),
        get_transition_actions(),
        get_interaction_actions()
    };

    auto const action_sum = std::accumulate(actions.begin(), actions.end(), 0.0, [](auto const val, auto const& action)
    {
        return val + action.sum();
    });

    if (action_sum == 0.0)
    {
        return false;
    }

    auto r = utils::random_double(1.0);
    auto const proposed_time = 1.0 / action_sum * std::log(1.0 / r);

    m_now += proposed_time;

    auto sum_r = utils::random_double(action_sum);

    auto const* it = actions.cbegin();
    while (sum_r > it->sum())
    {
        sum_r -= it->sum();
        it = std::next(it);
        assert(sum_r >= 0.0);
    }

    it->call(sum_r);

    // prevent overflow and falsely generated json output
    if (m_step == std::numeric_limits<std::size_t>::max())
    {
        m_step = 1; //LCOV_EXCL_LINE
    }
    else
    {
        ++m_step;
    }

    return m_now <= m_settings.time();
}


auto SSA::get_delete_edge_actions() -> Actions
{
    auto actions = Actions{};
    auto const delete_action = [this] (auto const a, auto const b) { this->m_network.delete_edge(a, b); };

    for (auto const& deletion : m_network.get_edge_deletion_rates())
    {
        actions.add(deletion.rate, delete_action, deletion.from, deletion.to);
    }

    return actions;
}


auto SSA::get_create_edge_actions() -> Actions
{
    auto actions = Actions{};
    auto const create_action = [this] (auto const a, auto const b) { this->m_network.create_edge(a, b); };

    for (auto const& creation : m_network.get_edge_creation_rates())
    {
        actions.add(creation.rate, create_action, creation.from, creation.to);
    }

    return actions;
}


auto SSA::get_death_actions() -> Actions
{
    auto actions = Actions{};
    auto death_action = [this] (auto const id) { this->m_network.remove(id); };

    for (auto const& death : m_network.get_deaths())
    {
        actions.add(death.rate, death_action, death.identifier);
    }

    return actions;
}


auto SSA::get_birth_actions() -> Actions
{
    auto actions = Actions{};
    auto birth_action = [this] (auto const& state) { this->m_network.create(this->m_now, state); };

    for (auto const& birth : m_network.get_births())
    {
        actions.add(birth.rate, birth_action, birth.identifier);
    }
    
    return actions;
}


auto SSA::get_transition_actions() -> Actions
{
    auto actions = Actions{};
    auto transition_action = [this] (auto const id, auto const state) { this->m_network.change(this->m_now, id, state); };

    for (auto const& transition : m_network.get_transitions())
    {
        actions.add(transition.rate, transition_action, transition.from, transition.to);
    }

    return actions;
}


auto SSA::get_interaction_actions() -> Actions
{
    auto actions = Actions{};
    auto interaction_action = [this] (auto const id, auto const state) { this->m_network.change(this->m_now, id, state); };

    for (auto const& interaction : m_network.get_interactions())
    {
        actions.add(interaction.rate, interaction_action, interaction.from, interaction.to);
    }

    return actions;
}


// LCOV_EXCL_START
auto SSA::to_json() const -> std::string
{
    auto block = utils::json::Block{};

    block.add_number("simulation_time", m_now);
    block.add_json("network", m_network.to_json());
    return block.to_string();
}
// LCOV_EXCL_STOP

} // namespace algorithm

