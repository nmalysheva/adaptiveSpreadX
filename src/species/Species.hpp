#ifndef SPECIES_SPECIES_HPP_
#define SPECIES_SPECIES_HPP_

#include <configuration/SpeciesConfiguration.hpp>
#include <species/Individual.hpp>
#include <types/Distribution.hpp>

#include <map>
#include <string>


/// Generates factories that allow to create Individuals, according to the given configuration.
class Species final
{
  public:
    /// Setup factories, according to the configuration.
    Species(SpeciesConfiguration const& config);

    /*!
     * \brief Create a new Individual of species Name.
     *
     * The individual will be created according to the previously received configuration.
     * \note Throws, if name is unknown.
     *
     * \param name Name of the species of the individual.
     * \return Generated individual.
     */
    [[nodiscard]]
    auto create(std::string const& name) const -> Individual;

  private:
    struct ContactRates final
    {
        Distribution loose;
        Distribution create;
    };

    /// name -> loose-, new-rate
    std::map<std::string, ContactRates> m_factory{};
};

#endif

