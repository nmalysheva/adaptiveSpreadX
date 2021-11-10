#ifndef ALGORITHM_SSA_HPP_
#define ALGORITHM_SSA_HPP_

#include "Actions.hpp"
#include "AlgorithmImpl.hpp"
#include "Settings.hpp"
#include <network/ContactNetwork.hpp>
#include <utils/Json.hpp>


namespace algorithm
{

/*!
 * \brief Implementation of SSA (Gillespie algorithm).
 *
 * \see e.g. https://en.wikipedia.org/wiki/Gillespie_algorithm for more information.
 *
 * \note The SSA assumes that `Settings` contains valid data.
 */
class SSA final : public AlgorithmImpl
{
  public:
    /// Initilise with given settings and network.
    SSA(Settings const& settings, network::ContactNetwork& network);

    /*!
     * \brief Run the algorithm
     * 
     * Runs the simulation until it is over (time exceeded or nothing to do anymore)
     * Each simulation step consists of 3 phases:
     * 1. collect all possible events and set up the corresponding function calls
     * 2. calculate the new simulation time
     * 3. execute a random action
     *
     * All simulation data is written to given json object.
     *
     * \param json The json object.
     */
    auto run(utils::json::Block& json) -> void override;

  private:
    /// current time
    double m_now{0.0};

    /// simulation output step counter
    std::size_t m_step{0};

    /// network to work with
    network::ContactNetwork& m_network;

    /// the settings
    Settings const& m_settings;

    /*!
     * \brief Execute a single step of the algorithm.
     *
     * 1. collect all possible events and set up the corresponding function calls
     * 2. calculate the new simulation time
     * 3. execute a random action
     *
     * \param the maximum time the simulation is allowed to advance
     * \return the time this step needed (or -1 if nothing was performed)
     */
    [[nodiscard]]
    auto execute(double time_max) -> double;

    /// convert current state to a string representation
    [[nodiscard]]
    auto to_json() const -> std::string;

    /// set up the "delete edge" events
    [[nodiscard]]
    auto get_delete_edge_actions() -> Actions;

    /// set up the "create edge" events
    [[nodiscard]]
    auto get_create_edge_actions() -> Actions;

    /// set up the "remove a node" events 
    [[nodiscard]]
    auto get_death_actions() -> Actions;

    /// set up the "create a node" events
    [[nodiscard]]
    auto get_birth_actions() -> Actions;
   
    /// set up the "spontaneous change state of a node" events
    [[nodiscard]]
    auto get_transition_actions() -> Actions;
   
    /// set up the "change state of a node, depending on its connections" events
    [[nodiscard]]
    auto get_interaction_actions() -> Actions;
};

} // namespace algorithm

#endif

