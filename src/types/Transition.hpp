#ifndef TYPES_TRANSITION_HPP_
#define TYPES_TRANSITION_HPP_

#include "Distribution.hpp"
#include "NodeId.hpp"
#include "State.hpp"
#include "Tripel.hpp"


/// Typedef to represent "with a value drawn from Distribution "something" taking the first State into account will resultl in "somehing" else, that takes the second State into account
using TransitionDistribution = Tripel<Distribution, State, State>;

/// Typedef to represent "with given propability the node identified by NodeId will change to State"
using TransitionRate = Tripel<double, NodeId, State>;

/// Typedef to represent "with given propability the node with first NodeId will "interact" with the node identified by second NodeId
using InteractionRate = Tripel<double, NodeId, NodeId>;

#endif

