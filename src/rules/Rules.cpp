#include "Rules.hpp"
#include <utils/parser.hpp>

#include <cassert>

#include <iostream>

Rules::Rules(ConfigurationBlock const& block)
{
    for (auto const& b : block)
    {
        // todo proper test instead of misusing exceptions
        try
        {
            auto const [from, connected, arrow, to, delim, prop] = parse<std::string, std::string, std::string, std::string, std::string, Propability>(b);
            assert(arrow == "->");
            assert(delim == ":");
            auto const rule = InteractionRule{from, connected, to, prop};
            m_interactions.emplace_back(rule);
        }
        catch (parse_error const&)
        {
            auto const [from, arrow, to, delim, prop] = parse<std::string, std::string, std::string, std::string, Propability>(b);
            assert(arrow == "->");
            assert(delim == ":");
            
            if (from == "_")
            {
                auto const rule = BirthRule{to, prop};
                m_birth_rules.emplace_back(rule);
            }
            else if (to == "_")
            {
                auto const rule = DeathRule{from, prop};
                m_death_rules.emplace_back(rule);
            }
            else
            {
                auto const rule = TransitionRule{from, to, prop};
                m_transitions.emplace_back(rule);
            }
        }
    }
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


auto Rules::get_interactions() const -> std::vector<InteractionRule> const&
{
    return m_interactions;
}
