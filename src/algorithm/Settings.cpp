#include "Settings.hpp"

#include <utils/Parse.hpp>

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>


namespace algorithm
{

Settings::Settings(configuration::Configuration const& config)
{
    auto const section = config.get("Algorithm");
    if (not section)
    {
        throw std::logic_error{AlgorithmMissing};
    }

    for (auto const& data : section->get())
    {
        auto const [name, value] = utils::parse::to_types<std::string, std::string>(data);

        if (name == "use")
        {
            set_algorithm(value);
        }
        else if (name == "time")
        {
            set_time(value);
        }
        else if (name == "epsilon")
        {
            set_epsilon(value);
        }
        else if (name == "output")
        {
            set_output_step(value);
        }
        else
        {
            throw std::logic_error{UnknownEntry};
        }
    }
}


auto Settings::set_algorithm(std::string const& algo) -> void
{
    if (m_algo)
    {
        throw std::logic_error{DuplicateAlgorithm};
    }
            
    if (algo == "SSA")
    {
        m_algo = Algorithm::SSA;
    }
    else if (algo == "SSATAN-X")
    {
        m_algo = Algorithm::SSATANX;
    }
    else
    {
        throw std::logic_error{UnknownAlgorithm};
    }
}


auto Settings::algorithm() const noexcept -> Algorithm
{
    return m_algo.value_or(DefaultAlgorithm);
}


auto Settings::set_time(std::string const& time) -> void
{
    if (m_time)
    {
        throw std::logic_error{DuplicateTime};
    }

    m_time = utils::parse::to_double(time);
}


auto Settings::time() const -> double
{
    if (not m_time)
    {
        throw std::logic_error{TimeMissing};
    }
    
    return m_time.value();
}


auto Settings::set_epsilon(std::string const& epsilon) -> void
{
    if (m_epsilon)
    {
        throw std::logic_error{DuplicateEpsilon};
    }

    m_epsilon = utils::parse::to_double(epsilon);
}


auto Settings::epsilon() const noexcept -> double
{
    return m_epsilon.value_or(DefaultEpsilon);
}


auto Settings::set_output_step(std::string const& step) -> void
{
    if (m_output_step)
    {
        throw std::logic_error{DuplicateOutput};
    }

    auto const val = utils::parse::to_unsigned(step);

    const auto max = static_cast<std::size_t>(std::floor(std::log10(std::numeric_limits<std::size_t>::max())));
    if (val <= max)
    {
        constexpr auto base = 10ul;
        m_output_step = static_cast<std::size_t>(std::pow(base, val));
    }
}


auto Settings::output_step() const noexcept -> std::size_t
{
    return m_output_step.value_or(DefaultOutput);
}

} // namespace algorithm

