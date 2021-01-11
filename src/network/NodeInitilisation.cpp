#include <network/NodeInitilisation.hpp>

#include <utility>


NodeInitilisation::NodeInitilisation(std::string&& s, size_t const c) noexcept
    : state{std::move(s)}, count{c}
{
}


auto operator==(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool
{
    return lhs.state == rhs.state;
}


auto operator<(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool
{
    return lhs.state < rhs.state;
}

