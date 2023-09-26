#ifndef ALGORITHM_SSATANX_HPP_
#define ALGORITHM_SSATANX_HPP_

#include "Algorithm.hpp"
#include "Settings.hpp"
#include <network/ContactNetwork.hpp>
#include <utils/Json.hpp>


namespace algorithm
{

/*!
 * \brief Implementation of SSATAN-X.
 *
 * \note SSATAN-X assumes that `Settings` contains valid data.
 */
class SSATANX final : public Algorithm
{
  public:
    /// Initilise with given settings and network.
    SSATANX(Settings const& settings, network::ContactNetwork& network);

    /*!
     * \brief Run the algorithm
     * 
     * Runs the simulation until it is over (time exceeded or nothing to do anymore).
     * Each step consists of 2 phases:
     * 1. TauLeap algorithm is used to update the contact dynamics of the network
     * 2. one viral dynamic is executed
     *
     * See paper for detailed information.
     *
     * Simulation data is written to given json object.
     *
     * \param json the json object to which the log is written
     */
    auto run(utils::json::Block& json) -> void override;

  private:
    /// calculate the upper bound approximation for the look ahead time for the viral dynamics
    auto get_btl() -> double;
};

} // namespace algorithm

#endif

