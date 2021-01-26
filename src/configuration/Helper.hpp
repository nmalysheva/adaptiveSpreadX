#ifndef CONFIGURATION_HELPER_HPP_
#define CONFIGURATION_HELPER_HPP_

#include "Stream.hpp"

#include <string>


/// helper functions and constants used by configuration components
namespace configuration::helper
{

/*! 
 * \brief text markers for different kinds of content
 *
 * \note: The markers can be changed and adjusted to the configuration file.
 * E.g. ; instead of # for comments. (Don't forget to change the tests.)
 */
namespace marker
{
/// marks a comment (default #)
constexpr auto Comment = '#';
/// marks the start of a header (default [)
constexpr auto HeaderStart = '[';
/// marks the end of a header (default ])
constexpr auto HeaderEnd = ']';
} // namespace marker


/// Categories of a text line.
enum class Category
{
    /// line is a comment
    comment,
    /// line contains configuration data
    data,
    /// line indicates EOS is reached
    eos,
    /// line contains a section header
    header
};

/*!
 * \brief Classify a string for interpretation.
 *
 * The category is determined by the following logic:
 * - input is empty -> Category::eos
 * - input starts with marker::Comment -> Category::comment
 * - first character of input is marker::HeaderStart and last character is marker::HeaderEnd -> Category::header
 * - else -> Category::data
 *
 * \param str input to classify
 * \return the category of the input
 */
[[nodiscard]]
auto classify(std::string const& str) noexcept -> Category;


/*!
 * \brief Read a stream until the header of a section is found.
 *
 * Reads the given stream line by line and discards comments until the first section header is found.
 *
 * \note After function returns the data can be received by calling Stream::get_line().
 *
 * \throw configuration::Exception if configuration data are found (the configuration file is ill-formed)
 *
 * \param stream stream to read
 * \return true if a header was found, false if eos was reached.
 */
[[nodiscard]]
auto find_first_header(Stream& stream) -> bool;

/*!
 * \brief Extract the name of a section header.
 *
 * marker::Start and marker::End are removed from the given string.
 * E.g. `[Hello]` will return `Hello`.
 *
 * \throw configuration::Exception the header is empty: []
 *
 * \pre The input is a valid section header.
 *
 * \param str section header from the configuration file
 * \return str with marker::HeaderStart and marker::HeaderEnd removed.
 */
[[nodiscard]]
auto unheader(std::string const& str) -> std::string;

} // namespace configuration::helper

#endif

