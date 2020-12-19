#include <types/Propability.hpp>

#include <stdexcept>


auto Propability::validate(double const value) -> double
{
    if (value < 0.0)
    {
       throw std::out_of_range{"Propability < 0 (0%)"};
    }

    if (value > 1.0)
    {
        throw std::out_of_range{"Propability > 1.0 (100%)"};
    }

    return value;
}


Propability::Propability(double const value)
    : m_value{validate(value)}
{
}


Propability::operator double() const noexcept
{
    return m_value;
}

