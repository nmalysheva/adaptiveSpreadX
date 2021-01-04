#ifndef SSA_HPP_
#define SSA_HPP_

#include <ContactNetwork.hpp>
#include <rules/Rules.hpp>


class SSA final
{
  public:
    SSA() = delete;
    SSA(double const start, double const end, ContactNetwork& network, Rules& rules) noexcept;

    auto execute() -> bool;

  private:
    double m_now;
    double const m_end;
    ContactNetwork& m_network;
    Rules& m_rules;
};

#endif

