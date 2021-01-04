#ifndef RULES_RULES_HPP_
#define RULES_RULES_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <rules/TransitionRule.hpp>

#include <vector>


class Rules final
{
  public:
    /// Read rules from configuration file
    Rules(ConfigurationBlock const& block);

    auto get_transitions() const -> std::vector<TransitionRule> const&;

  private:
    // transitions : rules with 1 node: I -> S : 0.4
    std::vector<TransitionRule> m_transitions;
    // interactions : rules with 2 nodes: S,I -> I : 0.1
};

#endif

