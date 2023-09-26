#include "InteractionManager.hpp"

#include <utils/Algorithm.hpp>

#include <algorithm>
#include <cassert>


namespace network
{

InteractionManager::InteractionManager(Settings const& settings)
    : m_interactions(settings.interactions().begin(), settings.interactions().end())
{
}


auto InteractionManager::get_rates() const noexcept -> std::vector<InteractionRate> const&
{
    return m_rates;
}


auto InteractionManager::get_state(State const& from, State const& to) const -> State const&
{
    auto const it = std::find_if(m_interactions.begin(), m_interactions.end(), [&from, &to] (auto const& interaction)
    {
        return (interaction.from == from) and (interaction.connected == to);
    });

    assert(it not_eq m_interactions.end());

    return it->to;
}


auto InteractionManager::remove(NodeId const id) -> void
{
    auto const it = std::remove_if(m_rates.begin(), m_rates.end(), [id](auto const& rate)
    {
        return (rate.from) == id or (rate.to == id);
    });

    m_rates.erase(it, m_rates.end());
}


auto InteractionManager::remove(NodeId const from, NodeId const to) -> void
{
    auto const it = std::remove_if(m_rates.begin(), m_rates.end(), [from, to](auto const& rate)
    {
        return ((rate.from == from) and (rate.to == to)) or ((rate.from == to) and (rate.to == from));
    });

    m_rates.erase(it, m_rates.end());
}


auto InteractionManager::add(NodeId const first, NodeId const second, State const& state_1, State const& state_2) -> void
{
    add_impl(first, second, state_1, state_2);
    add_impl(second, first, state_2, state_1);
}


auto InteractionManager::add_impl(NodeId const from, NodeId const to, State const& from_state, State const& to_state) -> void
{
    auto it = std::find_if(m_interactions.begin(), m_interactions.end(), [&from_state, &to_state](auto const& interaction)
    {
        return (interaction.from == from_state) and (interaction.connected == to_state);
    });

    if (it == m_interactions.end())
    {
        return;
    }

    auto const rate = it->distribution.draw();
    utils::emplace_if_positive(m_rates, rate, from, to);
}
    

auto InteractionManager::max_rates() const -> std::vector<StateTransitionRate>
{
    auto rates = std::vector<StateTransitionRate>{};
    rates.reserve(m_interactions.size());

    for (auto const& interaction : m_interactions)
    {
        rates.emplace_back(interaction.distribution.max(), interaction.from, interaction.connected);
    }

    return rates;
}

} // namespace network

