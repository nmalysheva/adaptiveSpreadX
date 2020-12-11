#ifndef CONTACTNETWORK_HPP_
#define CONTACTNETWORK_HPP_

#include <configuration/ConfigurationBlock.hpp>
#include <species/Species.hpp>
#include <graph/GraphImpl.hpp>


class ContactNetwork final
{
  public:
    ContactNetwork() = delete;
    ContactNetwork(ConfigurationBlock const& config, Species const& s);

  private:
    Species const& m_species;
    std::map<size_t, Individual> m_population;
    Graph<GraphImpl> m_graph;
};

#endif

