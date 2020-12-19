#ifndef SPECIES_INDIVIDUAL_HPP
#define SPECIES_INDIVIDUAL_HPP

#include <types/Propability.hpp>

#include <string>



/// Individual of a species.
struct Individual final
{
    /// State of the individual.
    std::string const state;

    /// Propability rate of connecting to another individual.
    Propability const new_contact_rate;

    /// Propability rate of loosing connection to another individual.
    Propability const loose_contact_rate;
};

#endif

