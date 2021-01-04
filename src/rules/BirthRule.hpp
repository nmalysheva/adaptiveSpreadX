#ifndef RULES_BIRTH_RULE_HPP_
#define RULES_BIRTH_RULE_HPP_

#include <types/Propability.hpp>

#include <string>

/*!
 * \brief Custom Birth rule to create new nodes.
 *
 * A new node of to_state is created with given propability.
 */
struct BirthRule final
{
    /// State of the new node.
    std::string const to_state;

    /// Propability of this rule to happen.
    Propability const propability;
};

#endif

