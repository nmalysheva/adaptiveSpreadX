#include "Settings.hpp"

#include <stdexcept>
#include <utility>


namespace algorithm
{

namespace
{
template <typename C>
auto validate(C const& cont, typename C::value_type const& val, char const* const msg)
{
    auto const it = std::find(cont.begin(), cont.end(), val);
    if (it not_eq cont.end())
    {
        throw std::logic_error{msg};
    }
}
} // namespace


auto Settings::add_birth(State s, Distribution d) -> void
{
    auto tmp = Transformation{std::move(s), std::move(d)};
    validate(m_births, tmp, DuplicateBirth);
    m_births.emplace_back(std::move(tmp));
}

auto Settings::births() const noexcept -> std::vector<Transformation> const&
{
    return m_births;
}


auto Settings::add_death(State s, Distribution d) -> void
{
    auto tmp = Transformation{std::move(s), std::move(d)};
    validate(m_deaths, tmp, DuplicateDeath);
    m_deaths.emplace_back(std::move(tmp));
}


auto Settings::deaths() const noexcept -> std::vector<Transformation> const&
{
    return m_deaths;
}


auto Settings::add_transition(State f, State t, Distribution d) -> void
{
    auto tmp = Transition{std::move(f), std::move(t), std::move(d)};
    validate(m_transitions, tmp, DuplicateTransition);
    m_transitions.emplace_back(std::move(tmp));
}


auto Settings::transitions() const noexcept -> std::vector<Transition> const&
{
    return m_transitions;
}


auto Settings::add_interaction(State f, State c, State t, Distribution d) -> void
{
    auto tmp = Interaction{std::move(f), std::move(c), std::move(t), std::move(d)};
    validate(m_interactions, tmp, DuplicateInteraction);
    m_interactions.emplace_back(std::move(tmp));
}


auto Settings::interactions() const noexcept -> std::vector<Interaction> const&
{
    return m_interactions;
}


auto Settings::set_time(double const time) noexcept -> void
{
    m_time = time;
}


auto Settings::time() const noexcept -> double
{
    return m_time;
}

} // namespace algorithm


