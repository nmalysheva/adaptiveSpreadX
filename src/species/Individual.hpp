#ifndef SPECIES_INDIVIDUAL_HPP
#define SPECIES_INDIVIDUAL_HPP

#include <string>


/// Individual of a species.
struct Individual final
{
    /// State of the individual.
    std::string const state;

    /// Propability rate of connecting to another individual.
    double const new_contact_rate;

    /// Propability rate of loosing connection to another individual.
    double const loose_contact_rate;
};

#endif

