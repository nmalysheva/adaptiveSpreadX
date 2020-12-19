//
// Created by Malysheva, Nadezhda on 2019-07-28.
//

#include "ContactNetwork.h"
#include <random>
#include <lemon/full_graph.h>
#include <map>
#include <set>
#include <unistd.h>
#include <algorithm>
#include <lemon/adaptors.h>
#include <stdexcept>
#include <iostream>

void ContactNetwork::init(size_t nInfected, size_t nSusceptible, size_t nEdges, int maxContactsL, int MaxContactsU,
                          double transmRate, double newContRate, double looseContRate, double diagnRate, double dRate, double bRate)
{
    initRates(maxContactsL, MaxContactsU, transmRate, newContRate, looseContRate,  diagnRate, dRate, bRate);

    size_t  nPopulation = nInfected + nSusceptible;
    initComplementNetwork(nPopulation);

    size_t nInf = 0;

    /* rates of assemble and disassemble edges a sampled from distributions
     *
     */
    std::uniform_real_distribution<double> lcd(looseContactRate*0.8, looseContactRate);
    std::uniform_real_distribution<double> ncd(newContactRate*0.8, newContactRate);
    isContactsLimited = false;
    for (size_t i = 0; i < nPopulation; i ++)
    {
        size_t maxContacts = maxContactsL;
        if (maxContacts < nPopulation - 1)
        {
            isContactsLimited = true;
        }

        double dRate = deathRate;//deathRateRateDistribution(generator);

        double looseContRate = lcd(m_generator);

        double newContRate = ncd(m_generator);

        Specie::State st = Specie::S;
        //Specie sp = Specie(maxContacts, 0, dRate, newContRate, looseContRate, st);
        Specie sp = Specie(maxContacts, 0, 0, newContRate, looseContRate, st);
        //std::cout <<maxContacts << ", lcr = " << looseContRate << ", ncr =" << newContRate << std::endl;

        lemon::ListGraph::Node newNode = network.addNode();
        diagnosisRates[newNode] = 0;

        if (nInf < nInfected)
        {
            sp.changeState(Specie::I, 0);
            diagnosisRates[newNode] = diagnosisRate;
            sp.setDeathRate(dRate);
            nInf++;

        }

        population.emplace(nodeIdMap[newNode], sp);


    }

    int maxNumberOfEdges = complement.maxEdgeId();

    for (size_t i = 0; i < nEdges; i ++)
    {
        std::uniform_int_distribution<int> dist(0, maxNumberOfEdges);
        int edgeId = dist(m_generator);
        lemon::ListGraph::Edge cEdge = complement.edgeFromId(edgeId);
        if (complement.valid(cEdge) &&  getEdgeAdditionRate(cEdge) > 0)
        {
            addEdge(cEdge);
        }
    }
}


size_t  ContactNetwork::countByState(Specie::State st) const
{
    size_t result = 0;

    for (auto& it: population)
    {
        if (it.second.getState() == st)
        {
            result++;
        }
    }
    return result;
}


std::vector<std::pair<double, lemon::ListGraph::Edge>> ContactNetwork::getTransmissionRateSum() const
{
    std::vector<std::pair<double, lemon::ListGraph::Edge>> propCumSum;
    propCumSum.reserve(1e6 + 1);

    //element <0, INVALID>
    std::pair<double, lemon::ListGraph::Edge> invalidElem {0, lemon::ListGraph::Edge (lemon::INVALID)};
    propCumSum.push_back(invalidElem);

    size_t index = 1;
    for (lemon::ListGraph::EdgeIt eIt(network); eIt != lemon::INVALID; ++eIt)
    {
        double rate = transmissionRates[eIt];
        if (rate > 0)
        {
            propCumSum.emplace_back(propCumSum.at(index - 1).first + rate, eIt);
            index ++;
        }
    }
    propCumSum.shrink_to_fit();
    return propCumSum;

}

std::vector<std::pair<double, lemon::ListGraph::Node>> ContactNetwork:: getDiagnosisRateSum()const
{
    std::vector<std::pair<double, lemon::ListGraph::Node>> propCumSum;
    propCumSum.reserve(1e6 + 1);

    //element <0, INVALID>
    std::pair<double, lemon::ListGraph::Node> invalidElem {0, lemon::ListGraph::Node (lemon::INVALID)};
    propCumSum.push_back(invalidElem);

    size_t index = 1;
    for (lemon::ListGraph::NodeIt nIt(network); nIt != lemon::INVALID; ++nIt)
    {
        double rate = diagnosisRates[nIt];
        if (rate > 0)
        {
            propCumSum.emplace_back(propCumSum.at(index - 1).first + rate, nIt);
            index ++;
        }
    }
    propCumSum.shrink_to_fit();
    return propCumSum;
}

std::vector<std::pair<double, lemon::ListGraph::Edge>> ContactNetwork::getEdgeDeletionRateSum() const
{
    std::vector<std::pair<double, lemon::ListGraph::Edge>> propCumSum;
    propCumSum.reserve(1e6 + 1); //TODO ADJUST TO POPULATION SIZE

    //element <0, INVALID>
    std::pair<double, lemon::ListGraph::Edge> invalidElem {0, lemon::ListGraph::Edge (lemon::INVALID)};
    propCumSum.push_back(invalidElem);

    size_t index = 1;
    for (lemon::ListGraph::EdgeIt eIt(network); eIt != lemon::INVALID; ++eIt)
    {
        double rate = getEdgeDeletionRate(eIt);
        if (rate > 0)
        {
            propCumSum.emplace_back(propCumSum.at(index - 1).first + rate, eIt);
            //std::cout << "elem = " << elem.first << std::endl;
            index ++;
        }
    }
    propCumSum.shrink_to_fit();
    return propCumSum;

}


std::vector<std::pair<double, lemon::ListGraph::Edge>> ContactNetwork::getEdgeAdditionRateSum()const
{
    std::vector<std::pair<double, lemon::ListGraph::Edge>> propCumSum;
    propCumSum.reserve(1e6 + 1);

    //element <0, INVALID>
    std::pair<double, lemon::ListGraph::Edge> invalidElem {0, lemon::ListGraph::Edge (lemon::INVALID)};
    propCumSum.push_back(invalidElem);

    size_t index = 1;
    for (lemon::ListGraph::EdgeIt eIt(complement); eIt != lemon::INVALID; ++eIt)
    {
        double rate = getEdgeAdditionRate(eIt);
        if (rate > 0)
        {
            propCumSum.emplace_back(propCumSum.at(index - 1).first + rate, eIt);
            index++;
        }
    }
    propCumSum.shrink_to_fit();
    return propCumSum;
}

std::vector<std::pair<double, lemon::ListGraph::Node>> ContactNetwork::getDeathRateSum()const
{
    std::vector<std::pair<double, lemon::ListGraph::Node>> propCumSum;
    propCumSum.reserve(1e6 + 1);

    //element <0, INVALID>
    std::pair<double, lemon::ListGraph::Node> invalidElem {0, lemon::ListGraph::Node (lemon::INVALID)};
    propCumSum.push_back(invalidElem);

    size_t index = 1;
    for(lemon::ListGraph::NodeIt nIt(network); nIt!=lemon::INVALID; ++nIt)
    {
        uint32_t nodeUID = nodeIdMap[nIt];
        double rate = population.at(nodeUID).getDeathRate();

        propCumSum.emplace_back(propCumSum.at(index - 1).first + rate, nIt);
        index++;
    }

    propCumSum.shrink_to_fit();
    return propCumSum;

}

double ContactNetwork::getTransmissionRateLimit() const
{
    return transmissionRate;
}


double ContactNetwork::getBirthRateSum()const
{
    return birthRate;
}

size_t ContactNetwork::size() const
{
    return lemon::countNodes(network);
}

std::pair<int, int> ContactNetwork::addEdge(lemon::ListGraph::Edge &complementEdge)
{
    if (!complement.valid(complementEdge))
    {
        std::string msg = "ERROR: INVALID EDGE TO ADD!";
        throw std::domain_error(msg);
    }
    //nodes of the given edge in a complement graph
    lemon::ListGraph::Node complU = complement.u(complementEdge);
    lemon::ListGraph::Node complV = complement.v(complementEdge);

    // find respective nodes in network
    lemon::ListGraph::Node networkU = network.nodeFromId(complement.id(complU));
    lemon::ListGraph::Node networkV = network.nodeFromId(complement.id(complV));


    //erase given edge from complement graph
    complement.erase(complementEdge);

    //add new edge to the network with given transmission rate
    lemon::ListGraph::Edge newEdge = network.addEdge(networkU, networkV);

    std::pair<int, int> result = std::make_pair(network.id(networkU), network.id(networkV));

    //get IDs of nodes. Have type int because it was originally in the definition of the IdMap class
    int sourceUID = nodeIdMap[networkU];
    int targetUID = nodeIdMap[networkV];

    //for these nodes increase number of contacts
    population.at(sourceUID).incNumberOfContacts();
    population.at(targetUID).incNumberOfContacts();

    //calculate transmission rate
    double trRate = 0;
    if ((population.at(sourceUID).getState()  == Specie::S  && population.at(targetUID).getState() == Specie::I) ||
        (population.at(sourceUID).getState()  == Specie::I  && population.at(targetUID).getState() == Specie::S))
    {
        trRate = transmissionRate;
    }

    if ((population.at(sourceUID).getState()  == Specie::S  && population.at(targetUID).getState() == Specie::D) ||
        (population.at(sourceUID).getState()  == Specie::D  && population.at(targetUID).getState() == Specie::S))
    {
        //for edges Diagnosed "D"- Susceptible "S" transmission rate is 50% lower than for edges "I" - "S"
        trRate = transmissionRate * 0.5;
    }

    transmissionRates[newEdge] = trRate;
    return result;
}

std::pair<int, int> ContactNetwork::removeEdge(lemon::ListGraph::Edge &edge)
{
    if (!network.valid(edge))
    {
        std::string msg = "ERROR: INVALID EDGE TO DEL!";
        throw std::domain_error(msg);
    }
    //nodes of the given edge in a graph
    lemon::ListGraph::Node networkU = network.u(edge);
    lemon::ListGraph::Node networkV = network.v(edge);

    int sourceUID = network.id(networkU);
    int targetUID = network.id(networkV);

    std::pair<int, int> result = std::make_pair(sourceUID, targetUID);

    network.erase(edge); // erase from network

    // find respective nodes in complement graph
    lemon::ListGraph::Node complU = complement.nodeFromId(sourceUID);
    lemon::ListGraph::Node complV = complement.nodeFromId(targetUID);
    complement.addEdge(complU, complV);   // add to complement graph

    // after removing an edge decrease num. of actual contacts for each of the incident nodes
    population.at(sourceUID).decNumberOfContacts();
    population.at(targetUID).decNumberOfContacts();

    return result;

}

void ContactNetwork::removeNode(lemon::ListGraph::Node &node)
{
    //delete all edges to given node in network
    lemon::ListGraph::IncEdgeIt ieIt(network, node);
    while (ieIt != lemon::INVALID)
    {
        lemon::ListGraph::IncEdgeIt tmpIt = ieIt;
        lemon::ListGraph::Node oppositeNode = network.oppositeNode(node, ieIt);
        population.at(nodeIdMap[oppositeNode]).decNumberOfContacts();
        ++ieIt;
        network.erase(tmpIt);

    }

    //find respective node in complement graph
    lemon::ListGraph::Node complementNode = complement.nodeFromId(network.id(node));

    //delete all edges to given node in complement graph
    lemon::ListGraph::IncEdgeIt cieIt(complement, complementNode);
    while (cieIt != lemon::INVALID)
    {
        lemon::ListGraph::IncEdgeIt tmpIt = cieIt;
        ++cieIt;
        complement.erase(tmpIt);
    }

    int sourceUID = nodeIdMap[node];
    population.erase(sourceUID);
    network.erase(node);
    complement.erase(complementNode);

}

void ContactNetwork::executeTransmission(lemon::ListGraph::Edge & edge, double time)
{
    lemon::ListGraph::Node networkU = network.u(edge);
    lemon::ListGraph::Node networkV = network.v(edge);

    int sourceUID = nodeIdMap[networkU];
    int targetUID = nodeIdMap[networkV];

    lemon::ListGraph::Node infectedNode = [&]()
    {

        //TODO Fool check: only susceptible (S) can be infected (I), etc. Prevent all impossible transactions aka
        // D->I, etc. Is it really necessary here?
        if (population.at(sourceUID).getState()  == Specie::S)
        {
            population.at(sourceUID).changeState(Specie::I, time);
            population.at(sourceUID).setDeathRate(deathRate);
            return  networkU;
        }

        else if (population.at(targetUID).getState()  == Specie::S)
        {
            population.at(targetUID).changeState(Specie::I, time);
            population.at(targetUID).setDeathRate(deathRate);
            return networkV;
        }

        throw std::runtime_error("impossible transaction");
    }();

    diagnosisRates[infectedNode] = diagnosisRate;
    // no adaptivity
    for(lemon::ListGraph::IncEdgeIt ieIt(network, infectedNode); ieIt!=lemon::INVALID; ++ieIt)
    {
        transmissionRates[ieIt] = 0;
        lemon::ListGraph::Node neighbourNode = network.v(ieIt);
        uint32_t nodeUID = nodeIdMap[neighbourNode];
        if(population.at(nodeUID).getState() == Specie::S)
        {
            transmissionRates[ieIt] = transmissionRate;//transmitDistribution(generator);

        }
    }

}

void ContactNetwork::executeDiagnosis(lemon::ListGraph::Node & node, double time)
{
    int nodeUID = nodeIdMap[node];
    population.at(nodeUID).changeState(Specie::D, time);

    diagnosisRates[node] = 0;

    //adaptivity: as soon as diagnosed, cut all contacts and reduce
    //new contact rate to 30%
    lemon::ListGraph::IncEdgeIt ieIt(network, node);
    while (ieIt != lemon::INVALID)
    {
        lemon::ListGraph::Edge tmpIt(ieIt);
        ++ieIt;
        removeEdge(tmpIt);
    }
    population.at(nodeUID).setNewContactRate(
            population.at(nodeUID).getNewContactRate() * 0.3);

}

void ContactNetwork::executeDeath(lemon::ListGraph::Node & node)
{
    removeNode(node);

    // after removing node from the population decrease max. number of contacts for each specie.
    //TODO: may be not integrate max. num of contacts to the specie. Leave it in the network
    for (auto& it: population)
    {
        it.second.setMaxNumberOfContacts(it.second.getMaxNumberOfContacts() - 1);
    }

}


//TODO outdated
void ContactNetwork::executeBirth(double rStart, double rBound)
{
    double result = rStart;
    result += birthRate;
    if (result >= rBound)
    {
        auto const maxContacts = std::uniform_int_distribution<>{}(m_generator);
        double  dRate = deathRate;//deathRateRateDistribution(generator);
        double newContRate = newContactRate;//newContactRateDistribution(generator);
        double looseContRate = looseContactRate;//looseContactRateDistribution(generator);
        Specie sp = Specie(maxContacts, 0, dRate, newContRate, looseContRate);

        lemon::ListGraph::Node newNode = network.addNode();
        population.emplace(nodeIdMap[newNode], sp);

        newNode = complement.addNode();
    }

    if (! isContactsLimited)
    {
        isContactsLimited = false;
        for (auto& it: population)
        {
            if (it.second.getMaxNumberOfContacts() < population.size() - 1)
            {
                isContactsLimited = true;
                break;
            }

        }
    }
}

double  ContactNetwork::getEdgeAdditionRate(const lemon::ListGraph::Edge &complementEdge) const
{
    double result = 0;

    lemon::ListGraph::Node complU = complement.u(complementEdge);
    lemon::ListGraph::Node complV = complement.v(complementEdge);

    int sourceUID = nodeIdMap[network.nodeFromId(complement.id(complU))];
    double sourceRate = population.at(sourceUID).getNewContactRate();
    //sourceRate = sourceRate * (population.at(sourceUID).getMaxNumberOfContacts() - population.at(sourceUID).getNumberOfContacts());

    int targetUID = nodeIdMap[network.nodeFromId(complement.id(complV))];
    double targetRate = population.at(targetUID).getNewContactRate();

    // rate of adding an edge is a multiplication of rates of both incident nodes
    result = sourceRate * targetRate;
    return result;
}

double  ContactNetwork::getEdgeDeletionRate(const lemon::ListGraph::Edge &networkEdge) const
{
    lemon::ListGraph::Node networkU = network.u(networkEdge);
    lemon::ListGraph::Node networkV = network.v(networkEdge);

    int sourceUID = nodeIdMap[networkU];
    int targetUID = nodeIdMap[networkV];

    double sourceRate = population.at(sourceUID).getLooseContactRate();
    double targetRate = population.at(targetUID).getLooseContactRate();

    //double result = (sourceRate + targetRate) / 2;
    // rate of deleting an edge is a multiplication of rates of both incident nodes
    double result = sourceRate * targetRate;
    return result;

}

size_t  ContactNetwork::countEdges() const
{
    return lemon::countEdges(network);
}


std::vector<size_t> ContactNetwork::getDegreeDistribution() const
{
    std::vector<size_t> result;
    result.reserve(maxContactsLimitU); //reserving space for vector.

    for(lemon::ListGraph::NodeIt nIt(network); nIt!=lemon::INVALID; ++nIt)
    {
        size_t degree = 0 ;

        for(lemon::ListGraph::IncEdgeIt e(network, nIt); e!=lemon::INVALID; ++e)
        {
            degree++;
        }

        result.push_back(degree);
    }

    return result;
}



size_t ContactNetwork::getMaxContactsLimitOfInfected(double t)const
{
    size_t result = 0;

    for (auto& it: population)
    {
        if (it.second.getState() == Specie::State::I || it.second.getState() == Specie::State::D)
        {
            result += it.second.getNumberOfContactsLimit(t);
        }
    }
    return result;

}

size_t ContactNetwork::getMaxContactsLimitOfSusceptible(double t)const
{
    size_t result = 0;

    for (auto& it: population)
    {
        if (it.second.getState() == Specie::State::S)
        {
            result += it.second.getNumberOfContactsLimit(t);
            //std::cout << "max cont: " << it.second.getNumberOfContactsLimit(t) << std::endl;
        }
    }
    return result;

}

void ContactNetwork::initRates(int maxContactsL, int MaxContactsU, double transmRate, double newContRate, double looseContRate,
                               double diagnRate, double dRate, double bRate)
{
    maxContactsLimitL = maxContactsL;
    maxContactsLimitU = MaxContactsU;
    transmissionRate = transmRate;

    //std::uniform_real_distribution<> dis(0.5, newContRate);
    newContactRate = newContRate;//dis(generator);//newContRate;
    //std::uniform_real_distribution<> dis2(0.5, looseContRate);
    looseContactRate = looseContRate;
    deathRate = dRate;
    birthRate = bRate;
    diagnosisRate = diagnRate;
}

void ContactNetwork::initComplementNetwork(size_t nPopulation)
{
    lemon::FullGraph fullG(nPopulation);
    lemon::GraphCopy<lemon::FullGraph, lemon::ListGraph> cg(fullG, complement);
    cg.run();

}


lemon::ListGraph::Edge ContactNetwork::getComplementEdge(int a, int b)
{
    lemon::ListGraph::Node complU = complement.nodeFromId(a);
    lemon::ListGraph::Node complV = complement.nodeFromId(b);

    lemon::ListGraph::Edge e = lemon::findEdge(complement, complU, complV);
    return e;
}

lemon::ListGraph::Edge ContactNetwork::getEdge(int a, int b)
{

    lemon::ListGraph::Node networkU = network.nodeFromId(a);
    lemon::ListGraph::Node networkV = network.nodeFromId(b);
    lemon::ListGraph::Edge e = lemon::findEdge(network, networkU, networkV);
    return e;

}