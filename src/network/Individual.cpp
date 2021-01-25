#include "Individual.hpp"

#include <utility>

namespace network
{

Individual::Individual(State s, Distribution::value_type const n, Distribution::value_type const r)
    : m_state{std::move(s)}, m_new_contact_rate{n}, m_remove_contact_rate{r}
{
}


auto Individual::state() const noexcept -> State const&
{
    return m_state;
}


auto Individual::new_contact_rate() const noexcept -> Distribution::value_type
{
    return m_new_contact_rate;
}


auto Individual::remove_contact_rate() const noexcept -> Distribution::value_type
{
    return m_remove_contact_rate;
}

} // namespace network

