#ifndef CONFIGURATION_STREAM_HPP_
#define CONFIGURATION_STREAM_HPP_

#include <istream>
#include <string>


namespace configuration
{

/*!
 * \brief Reads data from any std::istream.
 *
 * Unlike direct reading from a std::istream this class
 * ignores empty lines.
 */
class Stream final
{
  public:
    /*!
     * \brief Initialise Stream with given std::istream.
     *
     * \throws StreamError input not readable
     */
    Stream(std::istream& input);
   
    /*!
     * \brief Checks whether the stream contains more not-empty lines.
     *
     * \see get_line() for more information on the data.
     *
     * \return true if a non-empty line was found.
     */ 
    [[nodiscard]]
    auto has_next_line() -> bool;

    /*!
     * \brief Return the line last found by has_next_line().
     *
     * If EOS is reached, the result will be an empty string.
     *
     * \return The last line read by has_next_line() or empty in case of EOS.
     */
    [[nodiscard]]
    auto get_line() const noexcept -> std::string const&;

  private:
    /// stream to read from
    std::istream& m_stream;

    /// data read
    std::string m_current_line{};
};

} // namespace configuration

#endif

