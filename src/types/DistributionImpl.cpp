#include "DistributionImpl.hpp"

#include <algorithm>
#include <stdexcept>


std::mt19937 DistributionImpl::Generator{std::random_device{}()};

constexpr auto Min = 0.0;
constexpr auto Max = 1.0;


FixedDistribution::FixedDistribution(double const value)
    : m_value{value}
{
    if (std::clamp(m_value, Min, Max) not_eq m_value)
    {
        throw std::invalid_argument{"Fixed distribution will yield value not in range [0,1]"};
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

    if (a < Min or b > Max)
    {
        throw std::invalid_argument{"Uniform distribution will yield values not in range [0,1]"};
    }
}

auto UniformDistribution::draw() -> double
{
    return m_dist(DistributionImpl::Generator);
}


NormalDistribution::NormalDistribution(double const m, double const s)
    : m_dist{m, s}
{
    //  50% of the values drawn are in the range [0.675*s +- m]
    constexpr auto Bound = 0.675;
    auto const b = Bound * s;
    if ((m + b) > Max or (m - b) < Min)
    {
        throw std::invalid_argument{"Normal distribution will yield most of its values out of range [0,1]"};
    }
}

auto NormalDistribution::draw() -> double
{
    auto const val = m_dist(DistributionImpl::Generator);
    if (std::clamp(val, Min, Max) not_eq val)
    {
        return draw(); //LCOV_EXCL_LINE
    }
    return val;
}


ExponentialDistribution::ExponentialDistribution(double const l)
    : m_dist{l}
{
}

auto ExponentialDistribution::draw() -> double
{
    auto const val = m_dist(DistributionImpl::Generator);
    if (val > Max)
    {
        return draw(); //LCOV_EXCL_LINE
    }
    return val;
}

