#include "DistributionImpl.hpp"

#include <algorithm>
#include <stdexcept>


constexpr auto Min = 0.0;


FixedDistribution::FixedDistribution(double const value)
    : m_value{value}
{
    if (m_value < 0.0)
    {
        throw std::invalid_argument{"Fixed distribution will yield a value < 0"};
    }

}

auto FixedDistribution::draw() -> double
{
    return m_value;
}


UniformDistribution::UniformDistribution(double const a, double const b)
    : m_dist{a, b}
{
    if (b < a)
    {
        throw std::invalid_argument{"Uniform distribution with a > b"};
    }

    if (a < Min)
    {
        throw std::invalid_argument{"Uniform distribution can yield values < 0"};
    }
}

auto UniformDistribution::draw() -> double
{
    return m_dist(m_generator);
}


NormalDistribution::NormalDistribution(double const m, double const s)
    : m_dist{m, s}
{
}

auto NormalDistribution::draw() -> double
{
    auto const val = m_dist(m_generator);
    if (val < Min)
    {
        return draw();
    }
    return val;
}


ExponentialDistribution::ExponentialDistribution(double const l)
    : m_dist{l}
{
}

auto ExponentialDistribution::draw() -> double
{
    return m_dist(m_generator);
}

