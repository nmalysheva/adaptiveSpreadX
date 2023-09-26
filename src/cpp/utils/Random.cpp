#include "Random.hpp"
#include <types/DistributionImpl.hpp>

#include <cassert>
#include <limits>


namespace utils
{

auto random_double(double const max) -> double
{
    assert(max > 0.0);

    constexpr auto min = std::numeric_limits<double>::min(); // the smallest value > 0.0
    return UniformDistribution{min, max}.draw();
}

}

