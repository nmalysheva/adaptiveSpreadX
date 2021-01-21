#include "Distribution.hpp"

#include <utils/Random.hpp>

#include <cctype>
#include <stdexcept>


Distribution::Distribution(char const distribution, value_type const a, value_type const b)
{
    switch (std::toupper(distribution))
    {
        case 'U':
            {
                if (not (a < b))
                {
                    throw std::invalid_argument{std::string{"Error: a > b at distribution "} + distribution};
                }
                m_draw = [a, b] () { return utils::random<>(a, b); };
            }
            break;
        default:
            throw std::invalid_argument{std::string{"Unknown distribution: "} + distribution};
    }
}


Distribution::Distribution(value_type const value)
    : m_draw{[value] { return value; }}
{
}


auto Distribution::operator()() const -> value_type
{
    return m_draw();
}


auto Distribution::draw() const -> value_type
{
    return m_draw();
}


auto Distribution::draw(unsigned const count) const -> value_type
{
    auto val = value_type{0};
    for (unsigned i = 0; i < count; ++i)
    {
        val += draw();
    }
    return val;
}

