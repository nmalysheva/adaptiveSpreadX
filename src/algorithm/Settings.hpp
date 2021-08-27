#ifndef ALGORITHM_SETTINGS_HPP_
#define ALGORITHM_SETTINGS_HPP_

#include "Interaction.hpp"
#include "Transformation.hpp"
#include "Transition.hpp"

#include <types/State.hpp>
#include <types/Distribution.hpp>

#include <cstddef>
#include <vector>


namespace algorithm
{

/*!
 * \brief Store settings needed for SSA / NSA.
 *
 * This class does not check whether all required values.
 * are set.
 * In this application `settings::Settings` takes care of the requirements.
 *
 * But it is checked that each action is only set once.
 * Otherwise `std::logic_error`s are thrown.
 *
 * See `Transformation`, `Transition`, and `Interaction` for definition of 
 * equality.
 */
class Settings final
{
  public:
    /// birth event already set
    static constexpr auto DuplicateBirth = "duplicate birth event";

    /// death event already set
    static constexpr auto DuplicateDeath = "duplicate birth event";
    
    /// transition already set
    static constexpr auto DuplicateTransition = "duplicate transition";
   
    /// interaction already set
    static constexpr auto DuplicateInteraction = "duplicate interaction";

    /*!
     * \brief Add a `Transformation` for a birth event.
     * \throws std::logic_error an equal `Transformation` was already added
     *
     * \param s state that will be created
     * \param d propability distribution of this event
     */
    auto add_birth(State s, Distribution d) -> void;

    /// return birth events
    [[nodiscard]]
    auto births() const noexcept -> std::vector<Transformation> const&;

    /*!
     * \brief Add a `Transformation` for a death event.
     * \throws std::logic_error an equal `Transformation` was already added
     *
     * \param s state that will be removed
     * \param d propability distribution of this event
     */
    auto add_death(State s, Distribution d) -> void;

    /// return death events
    [[nodiscard]]
    auto deaths() const noexcept -> std::vector<Transformation> const&;

    /*!
     * \brief Add a `Transition` event
     * \throws std::logic_error an equal `Transition` was already added
     *
     * \param f state that will be transformed
     * \param t resulting state
     * \param d propability distribution of this event
     */
    auto add_transition(State f, State t, Distribution d) -> void;

    /// return transitions
    [[nodiscard]]
    auto transitions() const noexcept -> std::vector<Transition> const&;
    
    /*!
     * \brief Add an `Interaction` event.
     * \throws std::logic_error an equal `Interaction` was already added
     *
     * \param f state that will be changed
     * \param c required connected state
     * \param t resulting state
     * \param d propability distribution of this event
     */
    auto add_interaction(State f, State c, State t, Distribution d) -> void;

    /// return interactions
    [[nodiscard]]
    auto interactions() const noexcept -> std::vector<Interaction> const&;

    /*!
     * \brief Set the simulation time.
     *
     * Multiple calls will override the time.
     *
     * \param time simulation time
     */
    auto set_time(double time) noexcept -> void;

    /// get simulation end time
    [[nodiscard]]
    auto time() const noexcept -> double;

    /*!
     * \brief Set output step.
     *
     * Multiple calls will override the output step.
     *
     * \param output step
     */
    auto set_output_step(std::size_t step) noexcept -> void;

    /// get step when output should happen
    [[nodiscard]]
    auto output_step() const noexcept -> std::size_t;

  private:
    /// simulation time
    double m_time{0};

    /// output step
    std::size_t m_output_step{0};

    /// birth events
    std::vector<Transformation> m_births{};

    /// death events
    std::vector<Transformation> m_deaths{};

    /// transitions
    std::vector<Transition> m_transitions{};

    /// interactions
    std::vector<Interaction> m_interactions{};
};

} // namespace algorithm

#endif

