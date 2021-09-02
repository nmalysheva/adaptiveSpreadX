#include "Distribution.hpp"

#include <utils/Random.hpp>

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>


using namespace std::string_literals;


namespace
{
auto check_throw(std::string const& name, unsigned const given, unsigned const required) -> void
{
    if (given not_eq required)
    {
        auto ss = std::stringstream{};
        ss << name << " distribution needs " << required << " parameters, " << given << " provided";
        throw std::invalid_argument{ss.str()};
    }
}
} // namespace


Distribution::Distribution(char const distribution, std::vector<value_type> const& params)
{
    switch (std::toupper(distribution))
    {
        case 'U':
            check_throw("Uniform", params.size(), 2);
            m_dist = std::make_shared<UniformDistribution>(params.front(), params.back());
            break;
        case 'N':
            check_throw("Normal", params.size(), 2);
            m_dist = std::make_unique<NormalDistribution>(params.front(), params.back());
            break;
        case 'E':
            check_throw("Exponential", params.size(), 1);
            m_dist = std::make_unique<ExponentialDistribution>(params.front());
            break;
        default:
            throw std::invalid_argument{"Unknown distribution: "s + distribution};
    }
}


Distribution::Distribution(value_type const value)
    : m_dist{std::make_unique<FixedDistribution>(value)}
{
}


auto Distribution::draw() const -> value_type
{
    return m_dist->draw();
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

