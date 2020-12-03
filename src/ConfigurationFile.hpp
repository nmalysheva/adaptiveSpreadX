#ifndef CONFIGURATIONFILE_HPP_
#define CONFIGURATIONFILE_HPP_

#include <string_view>

/*!
 * \brief Accesses and reads a given configuration file.
 *
 * The file is opened and its content structured.
 * The read data can afterwards be used to initialise the appropriate configuration objects.
 * \note The ConfigurationFile only knows the required structure of the data.
 * The content has to be checked by the individual users.
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

  private:
};

#endif

