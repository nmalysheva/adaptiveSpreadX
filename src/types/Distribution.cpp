#include "Distribution.hpp"

#include <utils/Random.hpp>

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

using namespace std::string_literals;


Distribution::Distribution(char const distribution, value_type const a, value_type const b)
{
    switch (std::toupper(distribution))
    {
        case 'U':
            {
                if (not (a < b))
                {
                    throw std::invalid_argument{"Error: a >= b at distribution "s + distribution};
                }

                if ((a < 0.0) or (b > 1.0))
                {
                    throw std::out_of_range{OutOfRange};
                }

                m_draw = [a, b] () { return utils::random<>(a, b); };
            }
            break;

        default:
            throw std::invalid_argument{"Unknown distribution: "s + distribution};
    }
}


Distribution::Distribution(value_type const value)
    : m_draw{[value] { return value; }}
{
    if (std::clamp(value, 0.0, 1.0) not_eq value)
    {
        throw std::out_of_range{OutOfRange};
    }
}


auto Distribution::draw() const -> value_type
{
    return m_draw();
}


auto Distribution::draw(unsigned const n) const -> value_type
{
    auto val = value_type{0};
    for (unsigned i = 0; i < n; ++i)
    {
        val += draw();
    }
    return val;
}

