#ifndef CONFIGS_SEGMENTS_HPP_
#define CONFIGS_SEGMENTS_HPP_

#include <iterator>
#include <regex>
#include <string_view>
#include <utility>


/*!
 * \brief Parse a text line (e.g. from configuration file) and structure the data into single segments.
 *
 * This class takes care of all text parsing and processing, to
 * allow the consumers (e.g. TimeConfig) easy access to the data.
 * Example: given "A B C" contains the three segments A, B, and C.
 */
class Segments final
{
  public:
    /// Segments needs input.
    Segments() = delete;

    /// Seperate a given string(_view) to segments.
    Segments(std::string_view const text);

    /// Get number of segments.
    auto size() const
    {
        auto const end = std::cregex_iterator{};
        return std::distance(m_begin, end);
    }

    auto begin() const
    {
        return m_begin;
    }

  private:
    /// Regex to detect segments separated by whitespaces.
    static std::regex const RGX;

    /// Iterator pointing to the first segment.
    std::cregex_iterator m_begin{};
};

#endif

