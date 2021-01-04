#ifndef RULES_INTERACTIONRULE_HPP_
#define RULES_INTERACTIONRULE_HPP_

#include <types/Propability.hpp>

#include <string>

/*!
 * \brief Representation of an interaction rule.
 *
 * With a given propability a state of a node is changed from from_state to to_state
 * if it is connected with a node of state connected_state.
 */
struct InteractionRule final
{
    /// State of node that can be affected by this rule.
    std::string const from_state;

    /// State of a connected node.
    std::string const connected_state;
    
    /// New state after rule is applied.
    std::string const to_state;

    /// Propability of this rule to happen.
    Propability const propability;
};

#endif

