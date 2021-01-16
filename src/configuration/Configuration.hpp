#ifndef CONFIGURATION_CONFIGURATION_HPP_
#define CONFIGURATION_CONFIGURATION_HPP_

#include <configuration/Exception.hpp>

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
     * \brief Get the read data for given header
     *
     * \throw Exception the header was not found
     * \note The constructor T(iterator, iterator) with `iterator::value_type == std::string`
     * must be available.
     *
     * \tparam T type of data structure that the read data will be assigned to.
     * \param header the header of the section's
     * \return object of T with the data assigned
     */
    template <typename T>
    auto get(std::string const& header) const -> T
    {
        auto const data = m_data.find(header);
        if (data == m_data.end())
        {
            throw Exception{error::UnknownSection};
        }

        auto const& entries = data->second;
        return T(entries.cbegin(), entries.cend());
    }

  private:
    /// headers -> data sets
    std::unordered_map<std::string, std::vector<std::string>> m_data{};

    /// insert str, return ptr to set of data
    auto insert_header(std::string const& str) -> std::vector<std::string>*;
};

} // namespace configuration

#endif

