#ifndef RULES_RULES_HPP_
#define RULES_RULES_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <rules/BirthRule.hpp>
#include <rules/DeathRule.hpp>
#include <rules/InteractionRule.hpp>
#include <rules/TransitionRule.hpp>

#include <vector>

/*!
 * \brief Collection of user defined rules.
 *
 * A rule changes the state of a single node.
 * If the node changes by itself, it is called a transition.
 * If the node changes due to the connection to another node, it is called an interaction.
 *
 * The rule is executed with a given propability.
 */
class Rules final
{
  public:
    /// Read rules from configuration file
    Rules(ConfigurationBlock const& block);
    
    /// Get birth rules
    auto get_birth_rules() const -> std::vector<BirthRule> const&;
   
    /// Get deatch rules
    auto get_death_rules() const -> std::vector<DeathRule> const&;

    /// Get all transitions
    auto get_transitions() const -> std::vector<TransitionRule> const&;

    /// Get all interactions
    auto get_interactions() const -> std::vector<InteractionRule> const&;

  private:
    /// Birth rules: create new node
    std::vector<BirthRule> m_birth_rules;

    /// Death rules: remove node
    std::vector<DeathRule> m_death_rules;

    /// Transitions : rules with 1 node: I -> S : 0.4
    std::vector<TransitionRule> m_transitions;

    // Interactions : rules with 2 nodes: S,I -> I : 0.1
    std::vector<InteractionRule> m_interactions;
};

#endif

