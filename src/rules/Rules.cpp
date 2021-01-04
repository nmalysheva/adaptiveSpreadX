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
        auto rule = TransitionRule{from, to, prop};
        m_transitions.emplace_back(rule);
    }
    //Later: handle death
    //TransitionRule z{"I", "_", Propability{0.1}};
    //m_transitions.emplace_back(z);
}
    
auto Rules::get_transitions() const -> std::vector<TransitionRule> const&
{
    return m_transitions;
}

