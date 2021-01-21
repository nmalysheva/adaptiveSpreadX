#ifndef CONFIGURATION_CONFIGURATION_HPP_
#define CONFIGURATION_CONFIGURATION_HPP_

#include <istream>
#include <string>
#include <unordered_map>
#include <vector>


namespace configuration
{

class Configuration final
{
  public:
    /// Initilisie object by opening the given stream.
    explicit Configuration(std::istream& input);

    /*!
     * \brief Get the read data.
     *
     * \return object of T with the data assigned
     */
    auto const& get() const noexcept
    {
        return m_data;
    }

  private:
    /// headers -> data sets
    std::unordered_map<std::string, std::vector<std::string>> m_data{};

    /// insert str, return ptr to set of data
    auto insert_header(std::string const& str) -> std::vector<std::string>*;
};

} // namespace configuration

#endif

