#include "Settings.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>


namespace algorithm
{

auto Settings::set_time(double const time) -> void
{
    if (m_time)
    {
        throw std::logic_error{DuplicateTime};
    }

    m_time = time;
}


auto Settings::time() const noexcept -> double
{
    return *m_time;
}


auto Settings::set_output_step(std::size_t const step) -> void
{
    if (m_output_step)
    {
        throw std::logic_error{DuplicateOutput};
    }

    const auto max = static_cast<std::size_t>(std::floor(std::log10(std::numeric_limits<std::size_t>::max())));
    if (step <= max)
    {
        constexpr auto base = 10ul;
        m_output_step = static_cast<std::size_t>(std::pow(base, step));
    }
}


auto Settings::output_step() const noexcept -> std::size_t
{
    constexpr auto NoOutput = 0ul;
    return m_output_step.value_or(NoOutput);
}

} // namespace algorithm

