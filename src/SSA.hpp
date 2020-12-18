#ifndef SSA_HPP_
#define SSA_HPP_

#include <ContactNetwork.hpp>


class SSA final
{
  public:
    SSA() = delete;
    SSA(double const start, double const end, ContactNetwork& network) noexcept;

    auto execute() -> bool;

  private:
    double m_now;
    double const m_end;
    ContactNetwork& m_network;
};

#endif

