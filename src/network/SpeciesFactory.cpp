#include <network/SpeciesFactory.hpp>

#include <cassert>
#include <utility>


SpeciesFactory::SpeciesFactory(std::string&& s, Distribution const& c, Distribution const& l) noexcept
: state{std::move(s)}, create{c}, loose{l}
{
}


auto SpeciesFactory::make() const -> Individual
{
    return Individual{state, create.draw(), loose.draw()};
}


auto SpeciesFactory::comparable(std::string state) noexcept -> SpeciesFactory
{
    auto const prop = Propability{0};
    auto const dist = Distribution{prop};
    return SpeciesFactory{std::move(state), dist, dist};
}


auto operator==(SpeciesFactory const& lhs, SpeciesFactory const& rhs) noexcept -> bool
{
    return lhs.state == rhs.state;
}


auto operator<(SpeciesFactory const& lhs, SpeciesFactory const& rhs) noexcept -> bool
{
    return lhs.state < rhs.state;
}

