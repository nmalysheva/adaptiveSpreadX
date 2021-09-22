#ifndef NETWORK_INDIVIDUAL_HPP_
#define NETWORK_INDIVIDUAL_HPP_

#include <types/Distribution.hpp>
#include <types/State.hpp>
#include <types/Transformation.hpp>

#include <vector>


namespace network
{

/*!
 * \brief Representation of an Individual.
 *
 * An `Individual` has `State` and gains or loses a connection to another `Individual` with
 * a given propability.
 */
struct Individual final
{
    /// simulation time of last state change
    double modification_time;

    /// state
    State state;
    
    /// gain connection propability
    Distribution::value_type new_contact_rate;
    
    /// remove connection propability
    Distribution::value_type remove_contact_rate;

    /// death propability
    Distribution::value_type death_rate;
    
    /// spontaneous state change rates.
    std::vector<TransformationRate> transition_rates;
};

} // namespace network

#endif

