#ifndef ALGORITHM_SSA_HPP_
#define ALGORITHM_SSA_HPP_

#include "ActionsFactory.hpp"
#include "Algorithm.hpp"
#include "Settings.hpp"
#include <network/ContactNetwork.hpp>
#include <utils/Json.hpp>

#include <optional>


namespace algorithm
{

/*!
 * \brief Implementation of SSA
 *
 * The SSA can be used stand-alone (via configuration) or is used (see step)
 * by SSATAN-X.
 *
 * \note The SSA assumes that `Settings` contains valid data.
 */
class SSA final : public Algorithm
{
  public:
    /// Initilise with given settings and network.
    SSA(Settings const& settings, network::ContactNetwork& network);

    /*!
     * \brief Run the algorithm
     * 
     * Runs the simulation until it is over (time exceeded or nothing to do anymore).
     * SSA takes all possible actions and performs a step. (see step)
     *
     * Simulation data is written to given json object.
     *
     * \param json the json object to which the log is written
     */
    auto run(utils::json::Block& json) -> void override;

    /*!
     * \brief Execute a single SSA step.
     *
     * A step consists of 2 phases:
     * 1. calculate the new simulation times and threshold depending on the given actions (SSA=all, SSATAN-X=only contact)
     * 2. execute a random action chosen by the threshold from 1
     *
     * \param time_max maximum simulation time
     * \param actions the actions to work with
     *
     * \return an optional double with the calculated time passed, if the algorithm did not exceed time_max
     */
    [[nodiscard]]
    static auto step(double time_max, ActionsFactory::ActionPtrCollection const& actions) -> std::optional<double>;
};

} // namespace algorithm

#endif

