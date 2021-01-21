#ifndef SETTINGS_ALGORITHM_HPP_
#define SETTINGS_ALGORITHM_HPP_

#include "algorithm/Interaction.hpp"
#include "algorithm/Transformation.hpp"
#include "algorithm/Transition.hpp"

#include <types/State.hpp>
#include <types/Distribution.hpp>

#include <map>
#include <vector>
#include <utility>


namespace settings
{

class Algorithm final
{
  public:
    auto add_birth(State s, Distribution const d) -> void
    {
        auto tmp = Transformation{std::move(s), d};
        auto const it = std::find(m_births.cbegin(), m_births.cend(), tmp);
        if (it not_eq m_births.cend())
        {
            throw "Birth state already set";
        }

        m_births.emplace_back(std::move(tmp));
    }

    auto const& births() const noexcept
    {
        return m_births;
    }

    auto add_death(State s, Distribution const d) -> void
    {
        auto tmp = Transformation{std::move(s), d};
        auto const it = std::find(m_deaths.cbegin(), m_deaths.cend(), tmp);
        if (it not_eq m_deaths.cend())
        {
            throw "Death state already set";
        }

        m_deaths.emplace_back(std::move(tmp));
    }

    auto const& deaths() const noexcept
    {
        return m_deaths;
    }
    
    auto add_transition(State f, State t, Distribution const d) -> void
    {
        auto tmp = Transition{std::move(f), std::move(t), d};
        auto const it = std::find(m_transitions.cbegin(), m_transitions.cend(), tmp);
        if (it not_eq m_transitions.cend())
        {
            throw "Transition for state already defined";
        }

        m_transitions.emplace_back(std::move(tmp));
    }

    auto const& transitions() const noexcept
    {
        return m_transitions;
    }
    
    auto add_interaction(State f, State c, State t, Distribution const d) -> void
    {
        auto tmp = Interaction{std::move(f), std::move(c), std::move(t), d};
        auto const it = std::find(m_interactions.cbegin(), m_interactions.cend(), tmp);
        if (it not_eq m_interactions.cend())
        {
            throw "Interaction for state and connected already defined";
        }

        m_interactions.emplace_back(std::move(tmp));
    }

    auto const& interactions() const noexcept
    {
        return m_interactions;
    }

    auto set_time(double const time) noexcept
    {
        m_time = time;
    }

    auto time() const noexcept -> double
    {
        return m_time;
    }

  private:
    double m_time;
    std::vector<Transformation> m_births{};
    std::vector<Transformation> m_deaths{};
    std::vector<Transition> m_transitions{};
    std::vector<Interaction> m_interactions{};
};

} // namespace settings

#endif

