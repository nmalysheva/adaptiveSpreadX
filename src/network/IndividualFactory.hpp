#ifndef NETWORK_INDIVIDUALFACTORY_HPP_
#define NETWORK_INDIVIDUALFACTORY_HPP_

#include "Individual.hpp"
#include "Settings.hpp"

#include <types/Distribution.hpp>
#include <types/State.hpp>
#include <types/Transformation.hpp>

#include <map>
#include <vector>


namespace network
{
/*!
 * \brief Factory to create `Individual`s.
 *
 * The factory creates individuals and populates their create / remove edge, death,
 * and tranisition rates by drawing from a given propability distributions.
 */
class IndividualFactory final
{
  public:

    /// Distributions needed to create an Individual
    struct Distributions final
    {
        /// Distribution for creating a new contact
        Distribution create_contact;

        /// Distribution for deleting a contact
        Distribution remove_contact;

        /// Distribution for dying
        Distribution death;

        /// Distributions for spontaneous state changes
        std::vector<TransformationDistribution> transitions;
    };

    /*!
     * \brief Initialise new factory.
     *
     * The factory receives all needed information from the given settings to
     * automatically create `Individual`s with given rates.
     *
     * \param settings The settings for the creation of individuals.
     */
    IndividualFactory(Settings const& settings);

    /*!
     * \brief Create individual using factory for given state
     *
     * \pre `state` was added
     *
     * \param time simulation time of creating this individual
     * \param state state of the new individual
     */
    [[nodiscard]]
    auto make(double time, State state) const -> Individual;

    /// Draw birth rates
    [[nodiscard]]
    auto draw_birth_rates() const -> std::vector<BirthRate>;

  private:
    /// State -> distributions for rates
    std::map<State, Distributions> m_factory{};
    
    /// brith rates
    std::vector<BirthRate> m_births{};

    /// collection of the transitions (spontaneously state change)
    [[nodiscard]]
    auto draw_transition_rates(State const& state) const -> std::vector<TransformationRate>;
};

} // namespace network

#endif

