#ifndef TYPES_TRANSFORMATION_HPP_
#define TYPES_TRANSFORMATION_HPP_

#include "Distribution.hpp"
#include "NodeId.hpp"
#include "Pair.hpp"
#include "State.hpp"

/// Typedef to represent "with value drawn from Distribution something happens, using the given State"
using TransformationDistribution = Pair<Distribution, State>;

/// Typedef to represent "with the given porpability the result of "something" is the given state" (e.g. state change of a node)
using TransformationRate = Pair<double, State>;

/// Typedef to represent a TransformationDistribution, with "something" is the creation of a new node with given state
using BirthDistribution = TransformationDistribution;

/// Typedef to represent a TransformationRate, with "something" is the creation of a new node with given state
using BirthRate = TransformationRate;

/// Typedef that is almost the same as BirthDistribution, but expresses the removal (death) of a node
using DeathDistribution = BirthDistribution;

/// Typedef to represent "with this propability the node with given NodeId is removed (dies)"
using DeathRate = Pair<double, NodeId>;

#endif

