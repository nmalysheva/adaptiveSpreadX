#include <types/Propability.hpp>

#include <algorithm>
#include <stdexcept>


Propability::Propability(double const value)
    : m_value{value}
{
    if (m_value not_eq std::clamp(value, 0.0, 1.0))
    {
        throw std::out_of_range{"Propability out of range [0, 1]."};
    }
}


Propability::operator double() const noexcept
{
    return m_value;
}


auto operator<(Propability const lhs, Propability const rhs) noexcept -> bool
{
    return static_cast<Propability::value_type> (lhs) < static_cast<Propability::value_type> (rhs);
}

