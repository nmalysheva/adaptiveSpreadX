#ifndef ALGORITHM_TRANSITION_HPP_
#define ALGORITHM_TRANSITION_HPP_

#include <types/Distribution.hpp>
#include <types/Factory.hpp>
#include <types/Propability.hpp>
#include <types/SortedCollection.hpp>
#include <types/State.hpp>


struct Transition final
{
    using ParserTypes = std::tuple<State, State, Distribution>;
    static constexpr auto CompareIndices = 2u;

    State from;
    State to;
    Propability rate;

    Transition(State&& f, State&& t, Propability const r) noexcept
        : from{std::move(f)}, to{std::move(t)}, rate{r}
    {
    }
};


using TransitionFactory = Factory<Transition>;
using TransitionFactories = SortedCollection<TransitionFactory>;

#endif

