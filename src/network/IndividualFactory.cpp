#include "IndividualFactory.hpp"

#include <utils/Algorithm.hpp>

#include <cassert>
#include <stdexcept>
#include <tuple>
#include <utility>


namespace network
{


    
IndividualFactory::IndividualFactory(Settings const& settings)
{
    auto transitions = std::map<State, std::vector<TransformationDistribution>>{};
    for (auto const& trans : settings.transitions())
    {
        transitions[trans.from].emplace_back(trans.rate, trans.to);
    }

    for (auto const& state : settings.states())
    {
        auto const fake_edge_modification = EdgeModificationDistribution{Distribution::Ignore, state};
        auto const c = settings.edge_creation_distributions().find(fake_edge_modification);
        assert(c not_eq settings.edge_creation_distributions().end());
        auto const r = settings.edge_removal_distributions().find(fake_edge_modification);
        assert(r not_eq settings.edge_removal_distributions().end());

        auto const fake_death = DeathDistribution{Distribution::Ignore, state};
        auto const death_it = settings.death_distributions().find(fake_death);
        auto d = death_it == settings.death_distributions().cend() ? Distribution::Ignore : death_it->rate;

        auto params = Distributions{c->rate, r->rate, std::move(d), transitions[state]};
        m_factory.emplace(state, std::move(params));
    }

    for (auto const& birth : settings.birth_distributions())
    {
        auto const rate = birth.rate.draw();
        utils::emplace_if_positive(m_births, rate, birth.identifier);
    }
}


auto IndividualFactory::make(double const time, State state) const -> Individual
{
    auto const it = m_factory.find(state);
    assert(it not_eq m_factory.end());
    auto transitions = draw_transition_rates(state);
    return Individual{time,
                      std::move(state), 
                      it->second.create_contact.draw(),
                      it->second.remove_contact.draw(),
                      it->second.death.draw(),
                      std::move(transitions)};
}


auto IndividualFactory::draw_transition_rates(State const& state) const -> std::vector<TransformationRate>
{
    auto const it = m_factory.find(state);
    assert(it not_eq m_factory.end());
    auto result = std::vector<TransformationRate>{};

    for (auto const& trans : it->second.transitions)
    {
        auto const val = trans.rate.draw();
        utils::emplace_if_positive(result, val, trans.identifier);
    }

    return result;
}
    

auto IndividualFactory::draw_birth_rates() const -> std::vector<BirthRate>
{
    return m_births;
}

} // namespace network

