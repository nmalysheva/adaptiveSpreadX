#ifndef ALGORITHM_ALGORITHMIMPL_HPP_
#define ALGORITHM_ALGORITHMIMPL_HPP_

#include <utils/Json.hpp>

namespace algorithm
{

class AlgorithmImpl
{
  public:
    virtual auto run(utils::json::Block& json) -> void = 0;
};

} // namespace algorithm

#endif

