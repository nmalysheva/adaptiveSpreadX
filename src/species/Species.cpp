#include <species/Species.hpp>

#include <cassert>


Species::Species(SpeciesConfiguration const& config)
{
    for (auto const& c : config.get())
    {
        auto const& name = c.first;
        auto const& [loose, create] = c.second;
        m_factory.emplace(name, ContactRates{loose, create});
    }
}


auto Species::create(std::string const& name) const -> Individual
{
    assert(m_factory.count(name) == 1);
    auto const rates = m_factory.at(name);
    return {name, rates.create.draw(), rates.loose.draw()};
}

