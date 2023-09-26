#ifndef ALGORITHM_TAULEAP_HPP_
#define ALGORITHM_TAULEAP_HPP_

#include "ActionsFactory.hpp"
#include <network/ContactNetwork.hpp>


namespace algorithm
{

class TauLeap final
{
  public:
    /// Initialise TauLeap for a network with given epsilon.
    TauLeap(network::ContactNetwork& network, double epsilon);

    /*!
     * \brief Perfrom tauleap to bulk update the contact dynamics.
     *
     * \param t_now current time
     * \param t_final end time
     */
    auto execute(double t_now, double t_final) -> void;

  private:
    /// ActionsFactory managing the contact dynamics
    ActionsFactory m_factory;

    /// Epsilon used for calculation of the leap conditions
    double const m_epsilon;
};

} // namespace algorithm

#endif

