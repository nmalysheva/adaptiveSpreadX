#ifndef CONFIGS_SIMULATIONCONFIG_HPP_
#define CONFIGS_SIMULATIONCONFIG_HPP_

#include <cstddef>
#include <regex>
#include <stdexcept>
#include <string_view>

/*!
 * \brief Coniguration for simulation data.
 *
 * A simulation starts at time t_begin and stops at t_end,
 * with t_begin < t_end.
 * The interval [t_begin, t_end] is separated into num_simulation steps.
 */
class SimulationConfig final
{
  public:
    /// Type of exception thrown
    using exception_type = std::invalid_argument;

    /// No default time range.
    SimulationConfig() = delete;

    /*!
     * \brief Initialise via string.
     *
     * The times and step are parsed from a string.
     * The string must be of the form "A B C", with 0 <= A < B, and C > 0.
     * For valid numbers for A and B see https://en.cppreference.com/w/cpp/string/basic_string/stof
     * For valid numbers for C see https://en.cppreference.com/w/cpp/string/basic_string/stol
     * If the string is not valid std::invalid_argument exception is throw.
     */
    SimulationConfig(std::string_view const line);

    /// \return start time
    [[nodiscard]]
    auto start() const -> double
    {
        return m_start;
    }

    /// \return end time
    [[nodiscard]]
    auto end() const -> double
    {
        return m_end;
    }

    /// \return umber of simulation steps
    [[nodiscard]]
    auto steps() const -> int
    {
        return m_steps;
    }

  private:
    /// Starting time.
    double m_start;

    /// Ending time.
    double m_end;

    /// Number of simulation steps.
    int m_steps;

    /*!
     * \brief Throw class specific exception.
     *
     * Throws an exception of exception_type with the given text "SimulationConfig: given_message".
     *
     * \param msg The error message.
     */
    [[noreturn]]
    auto raise(std::string&& msg) const -> void;

    /*!
     * \brief Verify correct number of parameters.
     *
     * If too many or not enough parameters are provided, the function will raise an exception.
     *
     * \param given Given number of parameters.
     */
    auto raise_on_parameter_error(std::ptrdiff_t const given) const -> void;

    /*!
     * \brief Parse segments to populate internal data.
     *
     * Parses the segments of the given text to populate the internal data.
     * No check is performed, but std lib function can potentially throw.
     *
     * \param segment Regex iterator to the first segment to parse.
     */
    auto parse_data(std::cregex_iterator segment) -> void;

    /*!
     * \brie Verify correct data content.
     *
     * The data is correct, if start and end are positive times, and start is < end.
     * If these conditions are not met, an exception is raised.
     */
    auto raise_on_data_error() const -> void;
};

#endif

