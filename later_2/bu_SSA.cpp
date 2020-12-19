//
// Created by Malysheva, Nadezhda on 2019-07-28.
//

#include "SSA.h"

#include "Utility.h"

#include <string>


void SSA::execute(double tStart, double tEnd, ContactNetwork &contNetwork,
                  const std::string & saveDegreeDistMode)
{
    m_dist_mode = saveDegreeDistMode;

    double time = tStart;

    snapshot_contact(time, contNetwork);
    snapshot_viral(time, contNetwork);

    uint32_t  nInf = contNetwork.countByState(Specie::State::I);

    std::vector<std::pair<double, lemon::ListGraph::Edge>> propDel;
    std::vector<std::pair<double, lemon::ListGraph::Edge>> propAdd;
    std::vector<std::pair<double, lemon::ListGraph::Edge>> propTransmit;
    std::vector<std::pair<double, lemon::ListGraph::Node>> propDiagnos;
    std::vector<std::pair<double, lemon::ListGraph::Node>> propDeath;

    std::unordered_map<std::string, double >propensities {
            {"edge_del", 0},
            {"edge_add", 0},
            {"transmission", 0},
            {"diagnosis", 0},
            {"death", 0},
            {"birth", 0},
    };

    while (time < tEnd)
    {
        propDel = contNetwork.getEdgeDeletionRateSum();
        propAdd = contNetwork.getEdgeAdditionRateSum();
        propTransmit = contNetwork.getTransmissionRateSum();
        propDiagnos = contNetwork.getDiagnosisRateSum();
        propDeath = contNetwork.getDeathRateSum();

        propensities.at("edge_del") = propDel.back().first;
        propensities.at("edge_add") = propAdd.back().first;

        propensities.at("transmission") = propTransmit.back().first;
        propensities.at("diagnosis") = propDiagnos.back().first;
        propensities.at("death") = propDeath.back().first;

        propensities.at("birth") = contNetwork.getBirthRateSum();

        double propensitieSum = 0;
        for (auto &it: propensities)
        {
            propensitieSum += it.second;
        }

        if (propensitieSum == 0)
        {
            time = tEnd;

            snapshot_contact(time, contNetwork);
            snapshot_viral(time, contNetwork);

            break;
        }
        double r = sampleRandUni(generator);
        double proposedTime = 1 / propensitieSum * std::log(1/r);
        if (time + proposedTime > tEnd )
        {
            time = tEnd;

            snapshot_contact(time, contNetwork);
            snapshot_viral(time, contNetwork);
            break;
        }
            
        time += proposedTime;
        r = sampleRandUni(generator);

        double pSum = 0;
        for (auto &it: propensities)
        {
            if (pSum + it.second >= propensitieSum * r)
            {
                executeReaction(contNetwork, it.first, pSum, propensitieSum * r, time, nInf,
                        propTransmit, propDiagnos, propDeath);
                break;
            }
            pSum += it.second;
        }
    }
}

void SSA::executeReaction(ContactNetwork & contNetwork, const std::string &reactId, double rStart,
        double rBound, double time, uint32_t &nInf,
        std::vector<std::pair<double, lemon::ListGraph::Edge>> &propTransmit,
        std::vector<std::pair<double, lemon::ListGraph::Node>> &propDiagnos,
        std::vector<std::pair<double, lemon::ListGraph::Node>> &propDeath)
{
    if (reactId == "edge_del" or reactId == "edge_add")
    {
        snapshot_contact(time, contNetwork);
        snapshot_contact(time, contNetwork);
    }
    else if (reactId == "transmission")
    {
        size_t index = binarySearch(propTransmit, 0, propTransmit.size() - 1, rStart, rBound);
        contNetwork.executeTransmission(propTransmit.at(index).second, time);
        nInf++;

        snapshot_viral(time, contNetwork);
        if (m_dist_mode == "v")
        {
            m_time_infections.push_back(time);
        }

    }

    else if (reactId == "diagnosis")
    {
        size_t index = binarySearch(propDiagnos, 0, propDiagnos.size() - 1, rStart, rBound);
        contNetwork.executeDiagnosis(propDiagnos.at(index).second, time);
        snapshot_viral(time, contNetwork);
    }

    else if (reactId == "death")
    {
        size_t index = binarySearch(propDeath, 0, propDeath.size() - 1, rStart, rBound);
        contNetwork.executeDeath(propDeath.at(index).second);
        nInf = contNetwork.countByState(Specie::State::I) + contNetwork.countByState(Specie::State::D);
        snapshot_viral(time, contNetwork);
    }

    else if (reactId == "birth")
    {
        //nothing so far
    }
}


auto SSA::snapshot_contact(double const time, ContactNetwork const& network) -> void
{
    if (m_dist_mode == "c")
    {
        m_time_steps.push_back(time);
        m_degree_dist.push_back(network.getDegreeDistribution());
    }
}


auto SSA::snapshot_viral(double const time, ContactNetwork const& network) -> void
{
    if (m_dist_mode == "v")
    {
        m_time_steps.push_back(time);
        m_degree_dist.push_back(network.getDegreeDistribution());

        m_population_state.at(Specie::I).push_back(network.countByState(Specie::I));
        m_population_state.at(Specie::D).push_back(network.countByState(Specie::D));
        m_population_state.at(Specie::S).push_back(network.countByState(Specie::S));

        m_num_transmit_edges.push_back(network.getTransmissionRateSum().size() - 1);
    }
}
