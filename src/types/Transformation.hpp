#ifndef TYPES_TRANSOFRMATION_HPP_
#define TYPES_TRANSOFRMATION_HPP_

#include <types/Distribution.hpp>
#include <types/ParameterTypes.hpp>

#include <string>
#include <string_view>
#include <vector>


struct Transformation final
{
    using ParserTypes = ParameterTypes<std::string, Distribution>;

    std::string const state;
    Distribution const distribution;

    Transformation(std::string&& s, Distribution const dist) noexcept;
};


using Transformations = std::vector<Transformation>;

[[nodiscard]]
auto operator==(Transformation const& lhs, Transformation const& rhs) noexcept -> bool;

#endif

