#ifndef TYPES_EDGEMODIFICATION_HPP_
#define TYPES_EDGEMODIFICATION_HPP_

#include "Distribution.hpp"
#include "NodeId.hpp"
#include "Pair.hpp"
#include "State.hpp"
#include "Tripel.hpp"

/// Typedef to represent "with value drawn from Distribution a node with certain State will gain / lose an edge"
using EdgeModificationDistribution = Pair<Distribution, State>;

/// Typedef to represent "with this propability an edge between these two nodes will be created / removed"
using EdgeModificationRate = Tripel<double, NodeId, NodeId>;

#endif

