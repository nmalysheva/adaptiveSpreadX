#ifndef ALGORITHM_SSATANX_HPP_
#define ALGORITHM_SSATANX_HPP_

#include "Actions.hpp"
#include "AlgorithmImpl.hpp"
#include "Settings.hpp"
#include <network/ContactNetwork.hpp>
#include <utils/Json.hpp>


namespace algorithm
{

class SSATANX final : public AlgorithmImpl
{
  public:
    /// Initilise with given settings and network.
    SSATANX(Settings const& settings, network::ContactNetwork& network);
    
    auto run(utils::json::Block& json) -> void override;

  private:
};

} // namespace algorithm

#endif

