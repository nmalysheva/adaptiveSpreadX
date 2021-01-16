#ifndef NETWORK_SETTINGS_HPP_
#define NETWORK_SETTINGS_HPP_

#include <network/EdgeInitilisation.hpp>
#include <network/Individual.hpp>
#include <network/NodeInitilisation.hpp>

#include <tuple>


namespace network
{

using Settings = std::tuple<EdgeInitilisations, NodeInitilisations, IndividualFactories>;

}

#endif

