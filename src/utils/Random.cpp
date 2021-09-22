#include "Random.hpp"

#include <cassert>
#include <limits>
#include <random>


namespace utils
{

auto random_double(double const max) -> double
{
    assert(max > 0.0);

    constexpr auto min = std::numeric_limits<double>::min(); // the smallest value > 0.0
    static auto generator = std::default_random_engine{std::random_device{}()};
    auto dist = std::uniform_real_distribution<double>(min, max);

    return dist(generator);
}

}

