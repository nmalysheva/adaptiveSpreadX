#ifndef CONFIGURATION_CONFIGURATIONFILE_HPP_
#define CONFIGURATION_CONFIGURATIONFILE_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <configuration/SpeciesConfiguration.hpp>

#include <list>
#include <map>
#include <string>
#include <string_view>


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

    /*!
     * \brief Returns a specific block of configuration.
     *
     * \param name Name of the configuration block.
     * \returns Block containing the read content, or an empty Block if not available.
     */
    auto get_config(std::string const& name) const -> ConfigurationBlock const&;


    auto get_species() const -> SpeciesConfiguration const&;

  private:
    /// Stored configurations.
    std::map<std::string, ConfigurationBlock> m_data;

    SpeciesConfiguration m_species;
};

#endif

