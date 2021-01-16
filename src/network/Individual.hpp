#ifndef NETWORK_INDIVIDUALFACTORY_HPP_
#define NETWORK_INDIVIDUALFACTORY_HPP_

#include <types/Distribution.hpp>
#include <types/Factory.hpp>
#include <types/Propability.hpp>
#include <types/SortedCollection.hpp>
#include <types/State.hpp>


class Individual final
{
  public:
    constexpr static auto Header = "Species";
    using ParserTypes = std::tuple<State, Distribution, Distribution>;
    static constexpr auto CompareIndices = 1u;

    State state;
    Propability new_contact_rate;
    Propability loose_contact_rate;

    Individual(State&& s, Propability const n, Propability const l) noexcept
        : state{std::move(s)}, new_contact_rate{n}, loose_contact_rate{l}
    {
    }
};

using IndividualFactory = Factory<Individual>;
using IndividualFactories = SortedCollection<IndividualFactory>;

#endif

