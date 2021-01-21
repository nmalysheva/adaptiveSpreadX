#ifndef NETWORK_INDIVIDUALFACTORY_HPP_
#define NETWORK_INDIVIDUALFACTORY_HPP_

#include <types/Distribution.hpp>
#include <types/Factory.hpp>
#include <types/State.hpp>

#include <tuple>


class Individual final
{
  public:
    using CtorTypes = std::tuple<State, Distribution, Distribution>;
    static constexpr auto CompareIndices = 1u;

    State state;
    Distribution::value_type new_contact_rate;
    Distribution::value_type loose_contact_rate;

    Individual(State&& s, Distribution::value_type const n, Distribution::value_type const l) noexcept
        : state{std::move(s)}, new_contact_rate{n}, loose_contact_rate{l}
    {
    }
};

using IndividualFactory = Factory<Individual>;

#endif

