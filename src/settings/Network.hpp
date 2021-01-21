#ifndef SETTINGS_NETWORK_HPP_
#define SETTINGS_NETWORK_HPP_

#include <types/State.hpp>
#include <network/Individual.hpp>

#include <cstddef>
#include <limits>
#include <vector>


#include <map>


namespace settings
{

class Network final
{
  public:
    auto add_node(State s, std::size_t const count) -> void
    {
        for (auto const& p : m_nodes)
        {
            if (p.first == s)
            {
                throw "node count already defined";
            }
        }

        m_nodes.emplace_back(std::move(s), count);
    }


    auto const& nodes() const noexcept
    {
       return m_nodes;
    }


    auto set_edges(std::size_t const count) noexcept
    {
        m_edges = count;
    }


    auto edges() const noexcept
    {
        return m_edges;
    }


    auto add_factory(State const& s, Distribution const n, Distribution const l) -> bool
    {
        return m_factories.emplace(s, IndividualFactory{s, n, l}).second;
    }

    auto factories() const noexcept
    {
        return m_factories;
    }

  private:
    std::vector<std::pair<State, std::size_t>> m_nodes{};
    std::size_t m_edges{std::numeric_limits<std::size_t>::max()};
    std::map<State, IndividualFactory> m_factories{};
};

} // namespace settings

#endif

