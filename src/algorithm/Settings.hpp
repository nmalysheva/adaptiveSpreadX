#ifndef ALGORITHM_SETTINGS_HPP_
#define ALGORITHM_SETTINGS_HPP_

#include <cstddef>
#include <optional>


namespace algorithm
{

/*!
 * \brief Store settings needed for SSA / NSA.
 *
 * The class stores all settings needed to initialise / run the algorithms.
 * It makes sure that:
 *  - time is set exactly once
 *  - output_step is set at most once
 * On violation a `std::logic_error` with the corresponsing message is thrown.
 *
 * \note The simulation time is needed, but it is assumed that `settings::Settings` checks that the entry is available.
 */
class Settings final
{
  public:
    /// duplicate time
    static constexpr auto DuplicateTime = "duplicate time entry";

    /// duplicate output
    static constexpr auto DuplicateOutput = "duplicate output entry";

    /*!
     * \brief Set the simulation time.
     *
     * \throws std::logic_error if called more than once
     *
     * \param time simulation time
     */
    auto set_time(double time) -> void;

    /// get simulation end time
    [[nodiscard]]
    auto time() const noexcept -> double;

    /*!
     * \brief Set output step.
     *
     * \throws std::logic_error if called more than once
     *
     * \param output step
     */
    auto set_output_step(std::size_t step) -> void;

    /*!
     *  \brief Get step when output should happen.
     *
     * If the output_step was not set before, 0 is returned.
     *
     * \return Set step or 0.
     */
    [[nodiscard]]
    auto output_step() const noexcept -> std::size_t;

  private:
    /// simulation time
    std::optional<double> m_time{std::nullopt};

    /// output step
    std::optional<std::size_t> m_output_step{std::nullopt};
};

} // namespace algorithm

#endif

