#include <types/Percentage.hpp>

#include <stdexcept>


Percentage::Percentage(double const value)
    : m_value{value}
{
    check();
}


auto Percentage::check() const -> void
{
    if (m_value < 0.0)
    {
       throw std::out_of_range{"Percentage < 0 (0%)"};
    }

    if (m_value > 1.0)
    {
        throw std::out_of_range{"Percentage > 1.0 (100%)"};
    }
}

