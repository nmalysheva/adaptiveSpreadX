#ifndef SSA_HPP_
#define SSA_HPP_

#include <network/ContactNetwork.hpp>
#include <settings/Algorithm.hpp>


namespace algorithm
{

class SSA final
{
  public:
    SSA(settings::Algorithm const& setings, network::ContactNetwork& network);

    auto execute() -> bool;

    auto run() -> void;

  private:
    double m_now{0.0};
    network::ContactNetwork& m_network;
    settings::Algorithm const& m_rules;
};

} // namespace algorithm

#endif

