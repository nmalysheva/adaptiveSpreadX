#ifndef ALGORITHM_TRANSITIONFACTORY_HPP_
#define ALGORITHM_TRANSITIONFACTORY_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <algorithm/Transition.hpp>
#include <types/Distribution.hpp>
#include <types/ParameterTypes.hpp>
#include <types/SortedCollection.hpp>

#include <string>


class TransitionFactory final
{
  public:
    using ParserTypes = ParameterTypes<std::string, std::string, std::string, Distribution>;

    std::string from;
    std::string to;
    Distribution distribution;

    TransitionFactory() = delete;
    TransitionFactory(std::string&& f, std::string&& delim, std::string&& t, Distribution const& dist) noexcept;

    [[nodiscard]]
    auto make() const -> Transition;
};


using TransitionFactories = SortedCollection<TransitionFactory>;


[[nodiscard]]
auto operator<(TransitionFactory const& lhs, TransitionFactory const& rhs) noexcept -> bool;


[[nodiscard]]
auto operator==(TransitionFactory const& lhs, TransitionFactory const& rhs) noexcept -> bool;

#endif

