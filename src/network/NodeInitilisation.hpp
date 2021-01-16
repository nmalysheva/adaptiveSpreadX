#ifndef NETWORK_NODEINITILISATION_HPP_
#define NETWORK_NODEINITILISATION_HPP_

#include <types/SortedCollection.hpp>
#include <types/State.hpp>
#include <types/Unsigned.hpp>

#include <string>
#include <tuple>


class NodeInitilisation final
{
  public:
    static constexpr auto Header = "Nodes";

    NodeInitilisation() = delete;
    explicit NodeInitilisation(std::string const& str);

    [[nodiscard]]
    auto state() const noexcept -> State const&;

    [[nodiscard]]
    auto count() const noexcept -> Unsigned;

  private:
    std::tuple<State, Unsigned> m_data;
};


using NodeInitilisations = SortedCollection<NodeInitilisation>;


[[nodiscard]]
auto operator<(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool;


[[nodiscard]]
auto operator==(NodeInitilisation const& lhs, NodeInitilisation const& rhs) noexcept -> bool;

#endif

