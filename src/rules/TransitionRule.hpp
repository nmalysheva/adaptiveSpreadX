#ifndef RULES_TRANSITIONRULES_HPP_
#define RULES_TRANSITIONRULES_HPP_

#include <types/Propability.hpp>

/*!
 * \brief Representation of a transition.
 *
 * The rule changes the state from_state to to_state of a node
 * with the given propability.
 */
struct TransitionRule final
{
    /// State of node that can be affected by this rule.
    std::string const from_state;

    /// New state after rule is applied.
    std::string const to_state;

    /// Propability of this rule to happen.
    Propability const propability;
};

#endif

