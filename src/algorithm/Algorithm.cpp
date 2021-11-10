#include "Algorithm.hpp"
#include "SSA.hpp"
#include "SSATANX.hpp"


namespace algorithm
{

auto choose_algorithm(Settings const& settings, network::ContactNetwork& network) -> std::unique_ptr<AlgorithmImpl>
{
    switch (settings.algorithm())
    {
        case Settings::Algorithm::SSATANX:
            return std::make_unique<SSATANX>(settings, network);
        case Settings::Algorithm::SSA:
            [[fallthrough]];
        default:
            return std::make_unique<SSA>(settings, network);
    }
}

} // namespace algorithm
