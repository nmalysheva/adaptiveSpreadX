#include "Rules.hpp"
#include <utils/parser.hpp>

#include <cassert>


Rules::Rules(ConfigurationBlock const& block)
{
    for (auto const& b : block)
    {
        auto const [from, arrow, to, delim, prop] = parse<std::string, std::string, std::string, std::string, Propability>(b);
        assert(arrow == "->");
        assert(delim == ":");
        if (from == "_")
        {
            auto rule = BirthRule{to, prop};
            m_birth_rules.emplace_back(rule);
        }
        else if (to == "_")
        {
            auto rule = DeathRule{from, prop};
            m_death_rules.emplace_back(rule);
        }
        else
        {
            auto rule = TransitionRule{from, to, prop};
            m_transitions.emplace_back(rule);
        }
    }
    //Later: handle death
    //TransitionRule z{"I", "_", Propability{0.1}};
    //m_transitions.emplace_back(z);
}


auto Rules::get_birth_rules() const -> std::vector<BirthRule> const&
{
    return m_birth_rules;
}


auto Rules::get_death_rules() const -> std::vector<DeathRule> const&
{
    return m_death_rules;
}


auto Rules::get_transitions() const -> std::vector<TransitionRule> const&
{
    return m_transitions;
}

