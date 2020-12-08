#ifndef CONFIGURATIONFILE_HPP_
#define CONFIGURATIONFILE_HPP_

#include "configs/SimulationConfig.hpp"

#include <string>
#include <string_view>

/*!
 * \brief Accesses and reads a given configuration file.
 *
 * The file is opened and its content structured.
 * The read data can afterwards be used to initialise the appropriate configuration objects, by calling
 * the get_CONFIGURATION methods.
 *
 * The ConfigurationFile only knows the required structure of the data.
 * The content has to be checked by the individual configurations, when get_CONFIGRUATION is called.
 */
class ConfigurationFile final
{
  public:
    //! No default construction. A path must be provided.
    ConfigurationFile() = delete;

    /*!
     * \brief Open a configuration and read the data.
     *
     * On error a std::runtime_error with appropriate message is throwsn.
     * Errors are:
     * 1. no path provided
     * 2. unable to open file
     * 3. file is empty
     *
     * \param path The filepath to the configuration file.
     */
     ConfigurationFile(std::string_view const path);

     /*!
      * \brief Get the simulation time and step configuration.
      *
      * This methode parses the line of the configuration file that contains
      * the simulation time data.
      * \see SimulationConfig for restrictions and possible errors.
      *
      * \return Configuration of the simulation time.
      */
     [[nodiscard]]
     auto get_time_configuration() const -> SimulationConfig;

  private:
     /// Configuration string of simulation time.
     std::string m_time;
};

#endif

