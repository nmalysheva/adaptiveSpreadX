#include "IndividualFactory.hpp"

#include <cassert>
#include <stdexcept>
#include <utility>


namespace network
{

auto IndividualFactory::add(State state, Distribution const& create, Distribution const& remove) -> void
{
    auto const result = m_factory.try_emplace(std::move(state), create, remove);
    if (not result.second)
    {
        throw std::logic_error{DuplicateState};
    }
}


auto IndividualFactory::make(State state) const -> Individual
{
    auto const it = m_factory.find(state);
    assert(it not_eq m_factory.end());
    return Individual{std::move(state), it->second.first.draw(), it->second.second.draw()};
}

} // namespace network

