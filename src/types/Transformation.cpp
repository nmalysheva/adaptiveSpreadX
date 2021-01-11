#include <types/Transformation.hpp>

#include <utility>


Transformation::Transformation(std::string&& s, Distribution const dist) noexcept
    : state{std::move(s)}, distribution{dist}
{
}


auto operator==(Transformation const& lhs, Transformation const& rhs) noexcept -> bool
{
    return lhs.state == rhs.state;
}

