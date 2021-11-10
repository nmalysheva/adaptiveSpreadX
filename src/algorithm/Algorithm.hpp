#ifndef ALGORITHM_ALGORITHM_HPP_
#define ALGORITHM_ALGORITHM_HPP_

#include "AlgorithmImpl.hpp"
#include "Settings.hpp"
#include <network/ContactNetwork.hpp>
#include <memory>

namespace algorithm
{

/// \TODO: doc (choose algorithm according to settings)
auto choose_algorithm(Settings const& settings, network::ContactNetwork& network) -> std::unique_ptr<AlgorithmImpl>;

} // namespace algorithm

#endif

