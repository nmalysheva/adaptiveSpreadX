#include "SSA.hpp"

#include <utils/Random.hpp>

#include <array>
#include <cstddef>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>


namespace algorithm
{

SSA::SSA(Settings const& settings, network::ContactNetwork& network)
    : Algorithm{settings, network}
{
}


auto SSA::run(utils::json::Block& json) -> void
{
    auto now = 0.0;
    auto more = true;
    auto actions_factory = ActionsFactory{m_network};
    auto step = std::size_t{0};
    
    auto stats = utils::json::List<std::string>{};
    to_json(stats, now);

    while (more)
    {
        auto const time_max = m_settings.time() - now;

        auto const proposed_time = SSA::step(time_max, actions_factory.get_all(now));

        if (not proposed_time)
        {
            more = false;
        }
        else
        {
            now += proposed_time.value();
        }
        
        // prevent overflow and falsely generated json output
        if (step == std::numeric_limits<std::size_t>::max())
        {
            step = 0; //LCOV_EXCL_LINE
        }
        ++step;

        // store json if either output is desired or the algorithm is over
        if ((step == m_settings.output_step()) or (not more))
        {
            to_json(stats, now);
            step = 0;
        }
    }

    json.add_json("networks", stats.to_string());
}


auto SSA::step(double const time_max, ActionsFactory::ActionPtrCollection const& actions) -> std::optional<double>
{
    constexpr auto no_action = 0.0;
    auto const action_sum = std::accumulate(actions.begin(), actions.end(), no_action, [](auto const val, auto const& action)
            {
            return val + action->sum();
            });

    if (action_sum == no_action)
    {
        // nothing to do
        return std::nullopt;
    }

    auto const r = utils::random_double(1.0);
    auto const proposed_time = 1.0 / action_sum * std::log(1.0 / r);
    if (proposed_time > time_max)
    {
        // time would exceed the simulation
        return std::nullopt;
    }

    auto sum_r = utils::random_double(action_sum);
    auto it = actions.begin();
    while (sum_r > (*it)->sum())
    {
        sum_r -= (*it)->sum();
        it = std::next(it);
        assert(sum_r >= 0.0);
    }

    (*it)->call(sum_r);

    return std::make_optional(proposed_time);
}

} // namespace algorithm

