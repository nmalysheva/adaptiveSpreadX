#ifndef CONFIGURATION_CONFIGURATION_HPP_
#define CONFIGURATION_CONFIGURATION_HPP_

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>


namespace configuration
{

/*!
 * \brief Read data from a configuration file.
 *
 * The read data are only stored as a mapping of "section -> list of data".
 */
class Configuration final
{
  public:
    /*!
     * \brief Initilise object by opening the given stream.
     *
     * \attention See the README file for the correct format of the configuration file.
     *
     * \throw configuration::Exception on violation of the format
     */
    explicit Configuration(std::istream& input);

    /*!
     * \brief Get the read data.
     *
     * \return object of T with the data assigned
     */
    auto get() const noexcept -> auto const&
    {
        return m_data;
    }

    /*!
     * \brief Create Json representation of current data.
     *
     * Returns the read configuration in json format.
     *
     * \return json (string) of the data
     */
    auto to_json() const -> std::string;

  private:
    /// headers -> data sets
    std::unordered_map<std::string, std::vector<std::string>> m_data{};

    /// insert str, return ptr to set of data
    auto insert_header(std::string const& str) -> std::vector<std::string>*;
};

} // namespace configuration

#endif

