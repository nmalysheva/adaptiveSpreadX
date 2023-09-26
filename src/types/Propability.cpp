#include "Propability.hpp"

#include <algorithm>
#include <stdexcept>


Propability::Propability(double const val)
    : m_value{val}
{
    constexpr auto min = 0.0;
    constexpr auto max = 1.0;
    if (std::clamp(m_value, min, max) not_eq m_value)
    {
        throw std::invalid_argument{"Propability value not in the range [0.0, 1.0]"};
    }
}


Propability::operator double() const noexcept
{
    return m_value;
}

