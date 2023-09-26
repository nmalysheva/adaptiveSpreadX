#ifndef UTILS_JSON_HPP_
#define UTILS_JSON_HPP_

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <utility>


namespace utils::json
{

/*!
 * \brief Concatenate a list of data with comma.
 *
 *
 * Concatenate a list (i.e. vector) of given data with comma.
 * vector=[a,b,c] will be converted to "a,b,c".
 *
 * \note std::stringstream::operator<< for type T must be available.
 *
 * \tparam T type of data
 *
 * \param data The collection of data to concatenate.
 *
 * \return String like "a,b,c".
 */
template <typename T>
[[nodiscard]]
auto implode(std::vector<T> const& data) -> std::string
{
    if (data.empty())
    {
        return "";
    }

    auto s = std::stringstream{};
    auto e = std::prev(data.cend());
    std::for_each(data.cbegin(), e, [&s](auto const& d)
    {
        s << d << ',';
    });
    s << *e;
    return s.str();
}


/*!
 * \brief A json block element.
 *
 * Collection key-value-pairs, which is convertible to std::string.
 */
class Block final
{
  public:
    /// add a string: "name":"content"
    auto add_string(std::string const& name, std::string const& content) -> void;

    /// add a number: "name":number
    template <typename T>
    auto add_number(std::string const& name, T&& content) -> void
    {
        add_json(name, std::to_string(std::forward<T&&>(content)));
    }

    /// add json (same as add_string but content is not wrapped with "): "name":json
    auto add_json(std::string const& name, std::string const& content) -> void;

    /// convert passed data to a string representation: {"a":b,"c":"d","e":123}
    [[nodiscard]]
    auto to_string() const -> std::string;

  private:
    /// container holding passed data
    std::vector<std::string> m_data{};
};


/*!
 * \brief A json list element.
 *
 * Collection of values, which is convertible to std::string.
 *
 * \tparam Type of values to hold.
 */
template <typename T>
class List final
{
  public:
    /// store given data
    template <typename U>
    auto add(U&& content) -> void
    {
        m_data.emplace_back(static_cast<T> (content));
    }

    /// convert passed data to a string representation: [a,b,c]
    [[nodiscard]]
    auto to_string() const -> std::string
    {
        return std::string{'['} + implode(m_data) + ']';
    }

  private:
    /// container holding passed data
    std::vector<T> m_data{};
};

} // namespace utils::json

#endif 

