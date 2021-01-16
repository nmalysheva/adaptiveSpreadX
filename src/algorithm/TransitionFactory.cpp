#include <algorithm/TransitionFactory.hpp>

#include <cassert>
#include <tuple>


TransitionFactory::TransitionFactory(std::string&& f, [[maybe_unused]] std::string&& _, std::string&& t, Distribution const& d) noexcept
: from{f}, to{t}, distribution{d}
{
}

auto TransitionFactory::make() const -> Transition
{
    return Transition{from, to, distribution()};
}


auto operator<(TransitionFactory const& lhs, TransitionFactory const& rhs) noexcept -> bool
{
    return std::make_tuple(lhs.from, lhs.to) < std::make_tuple(rhs.from, rhs.to);
}


auto operator==(TransitionFactory const& lhs, TransitionFactory const& rhs) noexcept -> bool
{
    return std::make_tuple(lhs.from, lhs.to) == std::make_tuple(rhs.from, rhs.to);
}

