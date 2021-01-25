#ifndef SSA_HPP_
#define SSA_HPP_

#include "Settings.hpp"
#include <network/ContactNetwork.hpp>


namespace algorithm
{

class SSA final
{
  public:
    SSA(Settings const& setings, network::ContactNetwork& network);

    auto execute() -> bool;

    auto run() -> void;

  private:
    double m_now{0.0};
    network::ContactNetwork& m_network;
    Settings const& m_rules;
};

} // namespace algorithm

#endif

