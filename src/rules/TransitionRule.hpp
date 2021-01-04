#ifndef RULES_TRANSITIONRULES_HPP_
#define RULES_TRANSITIONRULES_HPP_

#include <types/Propability.hpp>


struct TransitionRule final
{
    std::string const from_state;
    std::string const to_state;
    Propability const propability;
};

#endif

