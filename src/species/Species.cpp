#include <species/Species.hpp>

#include <configuration/Parser.hpp>

#include <cassert>


Species::Species(ConfigurationBlock const& block)
{
    for (auto const& b : block)
    {
        auto const [name, nu, loose] = parse<std::string, std::string, std::string>(b);
        m_factory.emplace(name, AttributeFactory{nu, loose});
    }
}


auto Species::create(std::string const& name) const -> Individual
{
    assert(m_factory.count(name) == 1);
    auto const [n, l] = m_factory.at(name).create();
    return {name, n, l};
}

