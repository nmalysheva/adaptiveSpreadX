#include <types/State.hpp>


State::State(std::string const& name)
    : m_name{name}
{
}


auto State::operator()() const noexcept -> State const&
{
    return *this;
}


auto State::operator==(State const& rhs) const noexcept -> bool
{
    return m_name == rhs.m_name;
}


auto State::operator<(State const& rhs) const noexcept -> bool
{
    return m_name < rhs.m_name;
}

