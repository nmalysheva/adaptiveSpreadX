#ifndef ALGORITHM_INTERACTION_HPP_
#define ALGORITHM_INTERACTION_HPP_

#include <types/Distribution.hpp>
#include <types/Factory.hpp>
#include <types/Propability.hpp>
#include <types/SortedCollection.hpp>
#include <types/State.hpp>


struct Interaction final
{
    using ParserTypes = std::tuple<State, State, State, Distribution>;
    static constexpr auto CompareIndices = 2u;

    State from;
    State connected;
    State to;
    Propability rate;

    Interaction(State&& f, State&& c, State&& t, Propability const r) noexcept
        : from{std::move(f)}, connected{std::move(c)}, to{std::move(t)}, rate{r}
    {
    }
};


using InteractionFactory = Factory<Interaction>;
using InteractionFactories = SortedCollection<InteractionFactory>;

#endif

