#ifndef NETWORK_INDIVIDUAL_HPP_
#define NETWORK_INDIVIDUAL_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>


namespace network
{

/*!
 * \brief Representation of an Individual.
 *
 * An `Individual` has `State` and gains or loses a connection to another `Individual` with
 * a given propability.
 */
class Individual final
{
  public:
    /*!
     * \brief Create a new `Individual` with given state and connection propabilities.
     *
     * \param s state of the object
     * \param n propability for creating a new connection
     * \param r propability for removing an existing connection
     */
    Individual(State s, Distribution::value_type const n, Distribution::value_type const r);

    /// state of the individual
    auto state() const noexcept -> State const&;

    /// propability of creating new connection
    auto new_contact_rate() const noexcept -> Distribution::value_type;

    /// propability of removing a connection
    auto remove_contact_rate() const noexcept -> Distribution::value_type;
    
  private:
    /// state
    State m_state;

    /// gain connection propability
    Distribution::value_type m_new_contact_rate;

    /// remove connection propability
    Distribution::value_type m_remove_contact_rate;

};

} // namespace network

#endif

