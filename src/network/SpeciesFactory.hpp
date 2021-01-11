#ifndef NETWORK_SPECIESFACTORY_HPP_
#define NETWORK_SPECIESFACTORY_HPP_

#include <network/Individual.hpp>
#include <types/Distribution.hpp>
#include <types/SortedCollection.hpp>
#include <types/ParameterTypes.hpp>

#include <string>
#include <vector>


/// Generates factories that allow to create Individuals, according to the given configuration.
struct SpeciesFactory final
{
    using ParserTypes = ParameterTypes<std::string, Distribution, Distribution>;

    std::string  state;
    Distribution create;
    Distribution loose;

    SpeciesFactory(std::string&& s, Distribution const& c, Distribution const& l) noexcept;

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
    auto make() const -> Individual;

    [[nodiscard]]
    static auto comparable(std::string state) noexcept -> SpeciesFactory;
};

using SpeciesFactories = SortedCollection<SpeciesFactory>;


[[nodiscard]]
auto operator==(SpeciesFactory const& lhs, SpeciesFactory const& rhs) noexcept -> bool;


[[nodiscard]]
auto operator<(SpeciesFactory const& lhs, SpeciesFactory const& rhs) noexcept -> bool;

#endif

