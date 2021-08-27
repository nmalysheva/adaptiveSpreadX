#ifndef SSA_HPP_
#define SSA_HPP_

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
class SSA final
{
  public:
    /// Initilise with given settings and network.
    SSA(Settings const& settings, network::ContactNetwork& network);

    /// Execute a single step of the algorithm (time calculation and execute action).
    [[nodiscard]]
    auto execute() -> bool;

    /// Convenience function: calls execute until it returns false. Data written to given json object.
    auto run(utils::json::Block& json) -> void;

  private:
    /// current time
    double m_now{0.0};

    /// simulation output step counter
    std::size_t m_step{0};

    /// network to work with
    network::ContactNetwork& m_network;

    /// rule set
    Settings const& m_rules;

    /// convert current state to a string representation
    [[nodiscard]]
    auto to_json() const -> std::string;

};

} // namespace algorithm

#endif

