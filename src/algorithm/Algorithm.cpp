#include "Algorithm.hpp"
#include "SSA.hpp"
#include "SSATANX.hpp"


namespace algorithm
{

Algorithm::Algorithm(Settings const& settings, network::ContactNetwork& network)
    : m_network{network}, m_settings{settings}
{
}


// LCOV_EXCL_START
auto Algorithm::to_json(utils::json::List<std::string>& json, double const time, std::optional<unsigned> const thin) const -> void
{
    auto block = utils::json::Block{};

    block.add_number("simulation_time", time);
    if (thin)
    {
        block.add_number("thin", thin.value());
    }
    block.add_json("network", m_network.to_json());
    json.add(block.to_string());
}
// LCOV_EXCL_STOP


auto make_algorithm(Settings const& settings, network::ContactNetwork& network) -> std::unique_ptr<Algorithm>
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

