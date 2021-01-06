#ifndef SPECIES_INDIVIDUAL_HPP_
#define SPECIES_INDIVIDUAL_HPP_

#include <types/Propability.hpp>

#include <string>



/// Individual of a species.
struct Individual final
{
    /// State of the individual.
    std::string state;

    /// Propability rate of connecting to another individual.
    Propability new_contact_rate;

    /// Propability rate of loosing connection to another individual.
    Propability loose_contact_rate;
};

#endif

