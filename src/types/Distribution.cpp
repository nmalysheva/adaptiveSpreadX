#include <types/Distribution.hpp>

#include <utils/Random.hpp>

#include <cctype>
#include <stdexcept>


Distribution::Distribution(char const distribution, Propability const a, Propability const b)
{
    switch (std::toupper(distribution))
    {
        case 'U':
            {
                if (not (a < b))
                {
                    throw std::invalid_argument{std::string{"Error: a > b at distribution "} + distribution};
                }
                m_draw = [a, b] () { return random<>(a, b); };
            }
            break;
        default:
            throw std::invalid_argument{std::string{"Unknown distribution: "} + distribution};
    }
}


Distribution::Distribution(Propability const value)
    : m_draw{[value] { return value; }}
{
}


auto Distribution::draw() const -> Propability
{
    return m_draw();
}

