#ifndef CONTACTNETWORK_HPP_
#define CONTACTNETWORK_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <species/Individual.hpp>
#include <species/Species.hpp>
#include <graph/GraphImpl.hpp>

#include <map>
#include <vector>
#include <utility>


class ContactNetwork final
{
  public:
    ContactNetwork() = delete;
    ContactNetwork(ConfigurationBlock const& config, Species const& s);

    auto print() const -> void;

    auto get_edge_deletion_rates() const -> std::vector<std::pair<double, std::pair<size_t, size_t>>>;
    auto get_edge_creation_rates() const -> std::vector<std::pair<double, std::pair<size_t, size_t>>>;

    auto create_edge(size_t const from, size_t const to) -> void;
    auto delete_edge(size_t const from, size_t const to) -> void;

  private:
    Species const& m_species;
    std::map<size_t, Individual> m_population;
    Graph<GraphImpl> m_graph;
};

#endif

