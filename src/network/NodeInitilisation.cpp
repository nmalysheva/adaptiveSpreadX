#include <network/NodeInitilisation.hpp>

#include <utils/Parse.hpp>


NodeInitilisation::NodeInitilisation(std::string const& str)
    : m_data(parse::split<2>(str))
{
}
    
auto NodeInitilisation::state() const noexcept -> State const&
{
    return std::get<0>(m_data);
}

auto NodeInitilisation::count() const noexcept -> Unsigned
{
    return std::get<1>(m_data);
}


auto operator<(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool
{
    return lhs.state() < rhs.state();
}


auto operator==(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool
{
    return lhs.state() == rhs.state();
}

