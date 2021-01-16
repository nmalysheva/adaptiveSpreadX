#ifndef ALGORITHM_TRANSOFRMATION_HPP_
#define ALGORITHM_TRANSOFRMATION_HPP_

#include <types/Distribution.hpp>
#include <types/Factory.hpp>
#include <types/Propability.hpp>
#include <types/SortedCollection.hpp>
#include <types/State.hpp>


class Transformation final
{
  public:
    using ParserTypes = std::tuple<State, Distribution>;
    static constexpr auto CompareIndices = 1u;

    State state;
    Propability rate;

    Transformation(State&& s, Propability const r) noexcept
        : state{std::move(s)}, rate{r}
    {
    }
};

using TransformationFactory = Factory<Transformation>;
using TransformationFactories = SortedCollection<TransformationFactory>;

#endif

