#ifndef CONTACTNETWORK_HPP_
#define CONTACTNETWORK_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <graph/GraphImpl.hpp>
#include <species/Individual.hpp>
#include <species/Species.hpp>
#include <types/NodeId.hpp>

#include <map>
#include <vector>
#include <utility>


class ContactNetwork final
{
  public:
    ContactNetwork() = delete;
    ContactNetwork(ConfigurationBlock const& config, Species const& s);

    auto print() const -> void;

    auto get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;
    auto get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<NodeId, NodeId>>>;

    auto create_edge(NodeId const from, NodeId const to) -> void;
    auto delete_edge(NodeId const from, NodeId const to) -> void;

  private:
    Species const& m_species;
    std::map<NodeId, Individual> m_population;
    Graph<GraphImpl> m_graph;
};

#endif

