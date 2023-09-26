#ifndef ALGORITHM_SETTINGS_HPP_
#define ALGORITHM_SETTINGS_HPP_

#include <configuration/Configuration.hpp>

#include <cstddef>
#include <optional>


namespace algorithm
{

/*!
 * \brief Store settings needed for SSA / SSATAN-X
 *
 * The class stores all settings needed to initialise / run the algorithms.
 * It makes sure that:
 *  - time is set exactly once
 *  - output_step, algorithm and epsilon are set at most once
 * On violation a `std::logic_error` with the corresponsing message is thrown.
 */
class Settings final
{
  public:
    /*!
     * \brief Set up a Settings object with the data found in config.
     *
     * \throws std::logic_error if a value is set more than once or unknown
     * \throws what `Configuration::get()` throws
     */
    Settings(configuration::Configuration const& config);

    /// Possible algorithms to use
    enum class Algorithm
    {
        /// standard SSA
        SSA,

        /// optimisation using SSATAN-X
        SSATANX,

        /// unknown selection
        UNKNOWN
    };


    /// default algorithm
    static constexpr auto DefaultAlgorithm = Algorithm::SSA;

    /// default epsilon
    static constexpr auto DefaultEpsilon = 0.03;

    /// default output step (disabled)
    static constexpr auto DefaultOutput = 0ul;

    /// algorithm section missing
    static constexpr auto AlgorithmMissing = "section [Algorithm] missing";

    /// duplicate algorithm
    static constexpr auto DuplicateAlgorithm = "duplicate algorithm entry";

    /// algorithm entry is unknown
    static constexpr auto UnknownAlgorithm = "unknown algorithm selected";

    /// duplicate time
    static constexpr auto DuplicateTime = "duplicate time entry";

    /// time not set
    static constexpr auto TimeMissing = "time not set";

    /// duplicate epsilon
    static constexpr auto DuplicateEpsilon = "duplicate epsilon entry";

    /// duplicate output
    static constexpr auto DuplicateOutput = "duplicate output entry";

    /// unknown entry
    static constexpr auto UnknownEntry = "algorithm::Settings found unknown entry";


    /// get the algorithm (or default)
    [[nodiscard]]
    auto algorithm() const noexcept -> Algorithm;

    /// get simulation end time (throws if time was not set)
    [[nodiscard]]
    auto time() const -> double;

    /// get epsilong for tau-leap (or default)
    [[nodiscard]]
    auto epsilon() const noexcept -> double;

    /// get output step (or default)
    [[nodiscard]]
    auto output_step() const noexcept -> std::size_t;

  private:
    /// algorithm to use
    std::optional<Algorithm> m_algo{std::nullopt};

    /// simulation time
    std::optional<double> m_time{std::nullopt};

    /// epsilon
    std::optional<double> m_epsilon{std::nullopt};

    /// output step
    std::optional<std::size_t> m_output_step{std::nullopt};
    
    /*!
     * \brief Set the simulation time.
     *
     * \throws std::logic_error if called more than once
     *
     * \param time simulation time
     */
    auto set_time(std::string const& time) -> void;

    /*!
     * \brief Set the simulation epsilon.
     *
     * \throws std::logic_error if called more than once
     *
     * \param epsilon the epsilon for tau-leap
     */
    auto set_epsilon(std::string const& epsilon) -> void;

    /*!
     * \brief Set output step.
     *
     * \throws std::logic_error if called more than once
     *
     * \param output step
     */
    auto set_output_step(std::string const& step) -> void;
    
    /*!
     * \brief Set the alogorithm to use.
     *
     * \throws std::logic_error if called more than once
     *
     * \param algo the algorithm
     */
    auto set_algorithm(std::string const& algo) -> void;
};

} // namespace algorithm

#endif

