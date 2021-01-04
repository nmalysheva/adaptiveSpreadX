#ifndef RULES_DEATH_RULE_HPP_
#define RULES_DEATH_RULE_HPP_

#include <types/Propability.hpp>

#include <string>

/*!
 * \brief Custom Death rule to remove nodes.
 *
 * A node of from_state is removed with given propability.
 */
struct DeathRule final
{
    /// State of the affected node.
    std::string const from_state;

    /// Propability of this rule to happen.
    Propability const propability;
};

#endif

