#ifndef NETWORK_SETTINGS_HPP_
#define NETWORK_SETTINGS_HPP_

#include "IndividualFactory.hpp"

#include <types/Distribution.hpp>
#include <types/State.hpp>

#include <cstddef>
#include <set>
#include <vector>


namespace network
{

/*!
 * \brief Store settings needed for ContactNetwork.
 *
 * This class does not check whether all required values are set.
 * In this application `settings::Settings` takes care of the requirements.
 *
 * Each `add_*` function takes care, that no duplicates are set.
 * In case of a duplicate `std::logic_error`s are thrown.
 */
class Settings final
{
  public:
    /// state already present
    static constexpr auto DuplicateState = "duplicate node initilisation";

    /// unknown state
    static constexpr auto UnknownState = "node initilisation for unknown state";

    /*!
     * \brief Add a node initilisation for the network.
     * \throws std::logic_error state was already added
     * 
     * \param s state of node to create
     * \param count number of nodes (of state `s`) to create
     */
    auto add_node(State s, std::size_t count) -> void;

    /// return node initilisation
    [[nodiscard]]
    auto nodes() const noexcept -> std::vector<std::pair<State, std::size_t>> const&;

    /*!
     * \brief Initilise network with `count` edges.
     *
     * Multiple calls will override the edge count.
     *
     * \param count number of initial edges
     */
    auto set_edges(std::size_t count) noexcept -> void;

    /// get number of edges to start with
    [[nodiscard]]
    auto edges() const noexcept -> std::size_t;

    /*!
     * \brief Add an individual creation rule to the factory.
     * \throw see IndividualFactory::add()
     * 
     * \param s state to create
     * \param c create edge distribution
     * \param r remove edge distribution
     */
    auto add_factory(State s, Distribution const& c, Distribution const& r) -> void;

    /// get factory
    [[nodiscard]]
    auto factory() const noexcept -> IndividualFactory const&;

    /// get collection of all available states (= for which a factory is available)
    [[nodiscard]]
    auto states() const noexcept -> std::set<State> const&;
  
  private:
    /// number of initial edges
    std::size_t m_edges{0};

    /// defined states
    std::set<State> m_states{};

    /// node initilisation (state, count)
    std::vector<std::pair<State, std::size_t>> m_nodes{};

    /// factory to create individuals
    IndividualFactory m_factory{};
};

} // namespace settings

#endif

