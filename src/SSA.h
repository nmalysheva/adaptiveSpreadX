/* Created by Malysheva, Nadezhda on 2019-07-28.
 *
 *
 * Gillespie algorithm (SSA)
 * TODO:: make it STATIC
 */


#ifndef ALGO_SSA_H
#define ALGO_SSA_H



#include "ContactNetwork.hpp"

#include <cstdint>
#include <random>
#include <vector>

class SSA
{
public:
    void execute(double tStart, 
                 double tEnd, 
                 ContactNetwork &contNetwork,
                 const std::string &saveDegreeDist);


    auto const& get_time_steps() const
    {
        return m_time_steps;
    }

    auto const& get_time_infections() const
    {
        return m_time_infections;
    }

    auto const& get_transmit_edges() const
    {
        return m_num_transmit_edges;
    }

    auto const& get_degree_distribution() const
    {
        return m_degree_dist;
    }

    auto const& get_population_state() const
    {
        return m_population_state;
    }

private:

    void   executeReaction(ContactNetwork & contNetwork, const std::string &reactId, double rStart,
                           double rBound, double time, uint32_t &nInf,
                            std::vector<std::pair<double, lemon::ListGraph::Edge>> &propTransmit,
                            std::vector<std::pair<double, lemon::ListGraph::Node>> &propDiagnos,
                            std::vector<std::pair<double, lemon::ListGraph::Node>> &propDeath);

private:
    std::mt19937_64 generator{std::random_device{}()};

    /// List of all executed time steps.
    std::vector<double> m_time_steps;

    /// Times of infection.
    std::vector<double> m_time_infections;
   
    /// Number of trasmitting edges    
    std::vector<std::uint32_t> m_num_transmit_edges;

    /// Degree distribution of all nodes of the network at given step
    std::vector<std::vector<std::size_t>> m_degree_dist;
    
    /// Population state
    std::unordered_map<Specie::State, std::vector<uint32_t>> m_population_state
    {
        {Specie::I, {}},
        {Specie::D, {}},
        {Specie::S, {}}
    };

    std::string m_dist_mode;
    auto snapshot_contact(double const time, ContactNetwork const& network) -> void;
    auto snapshot_viral(double const time, ContactNetwork const& network) -> void;
};


#endif //ALGO_SSA_H
