#ifndef NETWORK_NODEINITILISATION_HPP_
#define NETWORK_NODEINITILISATION_HPP_

#include <types/SortedCollection.hpp>
#include <types/ParameterTypes.hpp>

#include <cstddef>
#include <string>
#include <vector>


struct NodeInitilisation final
{
    using ParserTypes = ParameterTypes<std::string, size_t>;

    std::string state;
    std::size_t count;

    NodeInitilisation(std::string&& s, size_t const c) noexcept;
};


using NodeInitilisations = SortedCollection<NodeInitilisation>;


[[nodiscard]]
auto operator==(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool;


[[nodiscard]]
auto operator<(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool;


#endif

