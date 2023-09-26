#include "Interaction.hpp"


namespace network
{

auto operator<(Interaction const& lhs, Interaction const& rhs) noexcept -> bool
{
    if (lhs.from == rhs.from)
    {
        return lhs.connected < rhs.connected;
    }

    return lhs.from < rhs.from;
}

} // namespace network
