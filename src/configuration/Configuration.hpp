#ifndef CONFIGURATION_CONFIGURATION_HPP_
#define CONFIGURATION_CONFIGURATION_HPP_

#include <functional>
#include <istream>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>


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
    /// typedef for list of entries
    using entry_list = std::list<std::string>;

    /// Typedef for the mapping HEADER -> entry_list
    using storage_type = std::unordered_map<std::string, entry_list>;

    /*!
     * \brief Initilise object by opening the given stream.
     *
     * \attention See the README file for the correct format of the configuration file.
     *
     * \throw configuration::Exception on violation of the format and rules
     */
    explicit Configuration(std::istream& input);

    /*!
     * \brief Get the read data.
     *
     * \param name header of the section
     * \return optional list of configuration entries
     */
    [[nodiscard]]
    auto get(std::string name) const -> std::optional<std::reference_wrapper<entry_list const>>;
    
    /*!
     * \brief Create Json representation of current data.
     *
     * Returns the read configuration in json format.
     *
     * \return json (string) of the data
     */
    [[nodiscard]]
    auto to_json() const -> std::string;

    /*!
     * \brief Check for unused configuration entries.
     * 
     * Every requested header is marked as "used".
     * After setup it can be checked, wheather data from the configuration file
     * are unused (that is unknown for the programme).
     * The name, if any, of the first section found will be returned.
     *
     * This is useful to check for typos, e.g. "Interactoins" instead of "Interactions".
     * 
     * \return an optional string, of the first unused header.
     */
    [[nodiscard]]
    auto get_unused() const noexcept -> std::optional<std::string>;

  private:
    /// headers -> data sets
    storage_type m_data{};

    /// used headers
    mutable std::unordered_set<std::string> m_used_headers{};

    /*!
     * \brief Insert a new header
     *
     * \throws configuration::Exception if the header is already in use
     *
     * \param name name of the header
     * \return iterator to the header, to add data
     */
    auto add_header(std::string&& name) -> storage_type::iterator;
};

} // namespace configuration

#endif

