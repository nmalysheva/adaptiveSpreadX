#ifndef ALGORITHM_ALGORITHM_HPP_
#define ALGORITHM_ALGORITHM_HPP_

#include "Settings.hpp"
#include <utils/Json.hpp>
#include <network/ContactNetwork.hpp>
#include <memory>


namespace algorithm
{

/*!
 * Base class for the algorithm implementations (SSA and SSATAN-X).
 *
 * \note The json data is collected throughout the whole program execution.
 * This can result in very large memory consumption.
 */
class Algorithm
{
  public:
    /*!
     * \brief Initialise all common data needed by the implementations:
     *
     * \param settings the algorithm settings parsed from the configuration file
     * \param network the network to work with
     */
    Algorithm(Settings const& settings, network::ContactNetwork& network);

    /*!
     * \brief Needs to be implemented: the actual execution of the algorithm.
     *
     * \param json The json structure to add output to.
     */
    virtual auto run(utils::json::Block& json) -> void = 0;

    /// default dtor
    virtual ~Algorithm() = default;

  protected:
    /// the network
    network::ContactNetwork& m_network;

    /// settings
    Settings const& m_settings;

    /// writes the algorithms data to the given json structure (opt. thin is used by SSATAN-X)
    auto to_json(utils::json::List<std::string>& json, double time, std::optional<unsigned> thin = std::nullopt) const -> void;
};


/*!
 * \brief Select and initalise the algorithm according to the settings.
 *
 * \brief settings algorithm settings used to select the algorithm and initialise it
 * \brief network the network the algorithm will work with
 *
 * \returns the algorithm (either SSA or SSATAN-X)
 */
auto make_algorithm(Settings const& settings, network::ContactNetwork& network) -> std::unique_ptr<Algorithm>;

} // namespace algorithm

#endif

