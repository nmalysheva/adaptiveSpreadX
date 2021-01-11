#ifndef CONFIGURATION_CONFIGURATIONFILE_HPP_
#define CONFIGURATION_CONFIGURATIONFILE_HPP_

#include <configuration/EdgeConfiguration.hpp>
#include <configuration/InteractionConfiguration.hpp>
#include <configuration/TransitionConfiguration.hpp>

#include <network/NodeInitilisation.hpp>
#include <network/SpeciesFactory.hpp>
#include <types/Transformation.hpp>

#include <string>
#include <string_view>
#include <vector>


/*!
 * \brief Accesses and reads a given configuration file.
 *
 * \todo Rework docs
 *
 * The file is opened and its content structured.
 * The read data can afterwards be used to initialise the appropriate configuration objects, by calling
 * get_configuration.
 *
 * The ConfigurationFile only clusters the content of the file.
 * It does not know about the required format for each component.
 *
 * The files is parsed by the following rules:
 * - an empty line is ignored
 * - a line beginning with # is ignored
 * - :NAME opens a block of name NAME
 * - anything else will be stored in the last opened block NAME
 * - everything until the first :NAME is ignored
 * Whitespaces in the beginning and end of the line will be ignored.
 *
 * Example file:
 * 1 2 3
 * # Hello, World!
 * :A
 * a b c
 *
 * :B
 * x y z
 *
 * Result:
 * 1 2 3 -> is ignored, because no block was opened
 * # Hello, World! -> is ignored, it is a comment
 * :A -> opens the Block A
 * a b c -> stored in block A
 * <empty line> -> is ignored
 * :B -> opens the Block B
 * x y z -> is stored in block B
 */
class ConfigurationFile final
{
  public:
    /// No default construction. A path must be provided.
    ConfigurationFile() = delete;

    /*!
     * \brief Open a configuration and read the data.
     *
     * On error a std::runtime_error with appropriate message is thrown.
     * Errors are:
     * 1. no path provided
     * 2. unable to open file
     * 3. file is empty
     *
     * \param path The filepath to the configuration file.
     */
    ConfigurationFile(std::string_view const path);


    auto get_edges() const -> EdgeConfiguration const&;
    auto get_network() const -> NodeInitilisations const&;
    auto get_species() const -> SpeciesFactories const&;
    auto get_deaths() const -> Transformations const&;
    auto get_births() const -> Transformations const&;
    auto get_interactions() const -> InteractionConfiguration const&;
    auto get_transitions() const -> TransitionConfiguration const&;

  private:
    EdgeConfiguration m_edges{};
    NodeInitilisations m_network{};
    SpeciesFactories m_species{};
    Transformations m_deaths{};
    Transformations m_births{};
    InteractionConfiguration m_interactions{};
    TransitionConfiguration m_transitions{};
};

#endif

