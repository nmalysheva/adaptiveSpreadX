#include "Settings.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>


namespace network
{

auto Settings::add_node(State s, std::size_t const count) -> void
{
    if (states().count(s) == 0)
    {
        throw std::logic_error{Settings::UnknownState};
    }

    auto const it = std::find_if(m_nodes.begin(), m_nodes.end(), [&s](auto const& pair)
    {
        return pair.first == s;
    });

    if (it not_eq m_nodes.end())
    {
        throw std::logic_error{Settings::DuplicateState};
    }

    m_nodes.emplace_back(std::move(s), count);
}


auto Settings::nodes() const noexcept -> std::vector<std::pair<State, std::size_t>> const&
{
    return m_nodes;
}


auto Settings::set_edges(std::size_t const count) noexcept -> void
{
    m_edges = count;
}


auto Settings::edges() const noexcept -> std::size_t
{
    return m_edges;
}


auto Settings::add_factory(State s, Distribution c, Distribution r) -> void
{
    m_factory.add(s, std::move(c), std::move(r));
    m_states.emplace(std::move(s));
}

auto Settings::factory() const noexcept -> network::IndividualFactory const&
{
    return m_factory;
}

auto Settings::states() const noexcept -> std::set<State> const&
{
    return m_states;
}

} // namespace settings

