#ifndef NETWORK_INDIVIDUALFACTORY_HPP_
#define NETWORK_INDIVIDUALFACTORY_HPP_

#include "Individual.hpp"

#include <types/Distribution.hpp>
#include <types/State.hpp>

#include <map>


namespace network
{
/*!
 * \brief Factory to create `Individual`s.
 *
 * The factory creates individuals and populates their create- and remove-rates 
 * by drawing from a given propability distribution.
 */
class IndividualFactory final
{
  public:
    /// Factory for state already set
    static constexpr auto DuplicateState = "factory for state already set";

    /*!
     * \brief Add a new factory for individual with given parameters.
     *
     * \throws std::logic_error a factory for the given state already exsits
     *
     * \param state state of the individual
     * \param create distribution for drawing create-contact-rates
     * \param remove distribution for drawing remove-contact-rates
     */
    auto add(State state, Distribution create, Distribution remove) -> void;

    /*!
     * \brief Create individual using factory for given state
     *
     * \pre `state` was added
     *
     * \pram state state of the new individual
     */
    auto make(State state) const -> Individual;

  private:
    /// State -> distribution pair
    std::map<State, std::pair<Distribution, Distribution>> m_factory{};
};

} // namespace network

#endif

