#ifndef SETTINGS_SETTINGS_HPP_
#define SETTINGS_SETTINGS_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "algorithm/Settings.hpp"
#include "network/Settings.hpp"


namespace settings
{

/*!
 * \brief Create setting objects for the algorithm and network.
 *
 * This object parses the data read e.g. from a configuration file and
 * sets up the settings for the algorithm and network.
 *
 * It checks that the configuration file is in a valid format:
 * - Species must be set
 * - Time must be set
 * - an entry line is in the wrong format
 * - a section is empty
 * - an unknown section appears
 * On violation `std::logic_error` is thrown.
 *
 * \note See `network::Settings` and `algorithm::Settings` and their used components
 * for detailed requirements / error checks / exceptions.
 */
class Settings final
{
  public:
    /// no species defined
    static constexpr auto NoSpecies = "no species set";

    /// time not defined
    static constexpr auto NoTime = "time not set";

    /// unknown section (+ name)
    static constexpr auto UnknownSection = "unknown section: ";

    /// empty section (+ name)
    static constexpr auto EmptySection = "empty section: ";

    /*!
     * \brief Parse read configuration and set up settings for the components.
     *
     * \param data map of the format "header -> [item 1, item 2, ...]"
     */
    explicit Settings(std::unordered_map<std::string, std::vector<std::string>> const& data);

    /// get the network settings
    [[nodiscard]]
    auto network() const noexcept -> network::Settings const&;

    /// get the algorithm settings
    [[nodiscard]]
    auto algorithm() const noexcept -> algorithm::Settings const&;

  private:
    /// settings for the network
    network::Settings m_network{};

    /// settings for the algorithm
    algorithm::Settings m_algorithm{};
};

} // namespace settings

#endif

