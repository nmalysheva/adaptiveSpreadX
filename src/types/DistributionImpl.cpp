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

auto FixedDistribution::max() const -> double
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

auto UniformDistribution::max() const -> double
{
    return m_dist.max();
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

// LCOV_EXCL_START
auto NormalDistribution::max() const -> double
{
    // independent from parameters
    return m_dist.max();
}
// LCOV_EXCL_STOP


ExponentialDistribution::ExponentialDistribution(double const l)
    : m_dist{l}
{
}

auto ExponentialDistribution::draw() -> double
{
    return m_dist(m_generator);
}

// LCOV_EXCL_START
auto ExponentialDistribution::max() const -> double
{
    // independent from parameters
    return m_dist.max();
}
// LCOV_EXCL_STOP


PoissonDistribution::PoissonDistribution(double const l)
    : m_dist{l}
{
    if (l <= 0)
    {
        throw std::invalid_argument{"Mean for PoissonDistribution not > 0"};
    }
}

auto PoissonDistribution::draw() -> double
{
    return m_dist(m_generator);
}

// LCOV_EXCL_START
auto PoissonDistribution::max() const -> double
{
    // independent from parameters
    return m_dist.max();
}
// LCOV_EXCL_STOP


BinomialDistribution::BinomialDistribution(unsigned const n, double const p)
    : m_dist{n, p}
{
    if (p < 0.0)
    {
        throw std::invalid_argument{"Propability for BinomialDistribution < 0"};
    }

    if (p > 1.0)
    {
        throw std::invalid_argument{"Propability for BinomialDistribution > 1"};
    }
}

auto BinomialDistribution::draw() -> double
{
    return m_dist(m_generator);
}

auto BinomialDistribution::max() const -> double
{
    return m_dist.max();
}
