#include "ContactNetwork.h"
#include "dynamics.hpp"
#include "SSA.h"

#include <chrono>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <iostream>

using namespace std;

void executeSSA(double tStart, double tEnd, size_t nInfected, size_t nSusceptible, size_t nEdges,int maxContactsA, int maxContactsB,
        double transmRate, double newContRate, double looseContRate, double diagnRate, double dRate, double bRate, size_t simulationNumber)
{
    ContactNetwork contNetwork(nInfected,
                               nSusceptible,
                               nEdges,
                               maxContactsA,
                               maxContactsB,
                               transmRate,
                               newContRate,
                               looseContRate,
                               diagnRate,
                               dRate,
                               bRate);

    //std::vector<BenStructure> benToFile = contNetwork.getBenStructure(tStart);
    size_t nPopulation = contNetwork.size();
    size_t startEdges = contNetwork.countEdges();
    std::cout <<"Nodes: "  << nPopulation <<std::endl;
    std::cout <<"Edges: "  << startEdges <<std::endl;

    std::vector<double> timeSteps;
    timeSteps.reserve(1e4 + 1);

    std::vector<std::vector<size_t>> degreeDistr; // vector stores degree dist. per time step
    degreeDistr.reserve(1e6 + 1);

    std::vector<double> timeInfection; // vector stores times of infection
    timeInfection.reserve(1e4 + 1);

    std::vector<uint32_t> numberOfTransmitEdges;// vector stores num. of edges eligible for transmission
    numberOfTransmitEdges.reserve(1e+4 + 1);

    std::unordered_map<Specie::State, std::vector<uint32_t>> populationState{
            {Specie::I, {}},
            {Specie::D, {}},
            {Specie::S, {}}
    };// map stores num. of species of each state

    SSA ssa;
    auto start_time = std::chrono::high_resolution_clock::now();
    ssa.execute(tStart, tEnd, contNetwork, timeSteps, timeInfection, populationState, numberOfTransmitEdges, degreeDistr, "v"/*, benToFile*/);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    char buffer[50];

    std::ostringstream strs;
    sprintf( buffer, "%.1e", newContRate);
    std::string newContRateStr(buffer);

    sprintf( buffer, "%.1e", looseContRate);
    std::string looseContRateStr(buffer);

    sprintf( buffer, "%.1e", diagnRate);
    std::string diagnRateStr(buffer);

    sprintf( buffer, "%.1e", dRate);
    std::string dRateStr(buffer);

    sprintf( buffer, "%.1e", transmRate);
    std::string transmRateStr(buffer);

    string fileName = "SSA_"  + std::to_string(nPopulation) + "_nInf_" + std::to_string(nInfected) +
                      "_MaxCont_" + std::to_string(maxContactsA) + "-" + std::to_string(maxContactsB) +
                      "_addR_" + newContRateStr + "_delR_" + looseContRateStr +
                      "_diagnR_" + diagnRateStr  + "_deathR_" + dRateStr +
                      "_trR_" + transmRateStr + "_" +
                      std::to_string(simulationNumber) + ".txt";

    ofstream newFile;
    newFile.open(fileName);

    //newFile << "duration in CPU time: " << time;
    newFile << "duration in milliseconds: " << chrono::duration <double, milli> (time).count() << std::endl;
    newFile << "population start: " << nPopulation << std::endl;
    newFile << "population end: " << contNetwork.size() << std::endl;
    newFile << "edges: " << startEdges << std::endl;
    newFile << "infected start: " << nInfected << std::endl;
    newFile << "infected end: " << contNetwork.countByState(Specie::I) + contNetwork.countByState(Specie::D) << std::endl;


    newFile << "max contact range: " << maxContactsA << " "  << maxContactsB << std::endl;
    newFile << "transmission rate: " << transmRate << std::endl;
    newFile << "rate of make a new contact: " << newContRate << std::endl;
    newFile << "rate of loose a contact: " << looseContRate << std::endl;
    newFile << "death rate: " << dRate << std::endl;
    newFile << "birth rate: " << bRate << std::endl;
    newFile << "diagnosis rate: " << diagnRate << std::endl;
    newFile << "transmission rate: " << transmRate << std::endl;

    newFile << "time steps: "<< std::endl;
    for (const auto &tStep: timeSteps)
    {
        newFile << tStep << ' ';
    }
    newFile << std::endl;

    newFile << "number of infected: "<< std::endl;
    for (size_t i = 0; i < populationState.at(Specie::I).size(); i++)
    {
        newFile << populationState.at(Specie::I).at(i) << ' ';
    }
    newFile << std::endl;

    newFile << "number of diagnosed: "<< std::endl;
    for (size_t i = 0; i < populationState.at(Specie::D).size(); i++)
    {
        newFile << populationState.at(Specie::D).at(i) << ' ';
    }
    newFile << std::endl;

    newFile << "number of susceptible: "<< std::endl;
    for (size_t i = 0; i < populationState.at(Specie::S).size(); i++)
    {
        newFile << populationState.at(Specie::S).at(i) << ' ';
    }
    newFile << std::endl;

    newFile << "infect. time: "<< std::endl;
    for (size_t i = 0; i < timeInfection.size(); i++)
    {
        newFile << timeInfection.at(i) << ' ';
    }
    newFile << std::endl;

    newFile << "number of edges with inf.: "<< std::endl;
    for (size_t i = 0; i < numberOfTransmitEdges.size(); i++)
    {
        newFile << numberOfTransmitEdges.at(i) << ' ';
    }
    newFile <<  std::endl;

    newFile << "degree distribution:" << std::endl;
    for (const auto &deegreDist: degreeDistr)
    {
        std::vector<size_t> degreeDatTime = deegreDist;
        for (size_t j = 0;  j < degreeDatTime.size(); j++)
        {
            newFile << degreeDatTime.at(j) << ' ';
        }
        newFile << std::endl;

    }


    newFile.close();

}


std::pair<double, double> getAndPrintSimulationParameters(char* timeBoundaries[], size_t &simulationNumber)
{
    simulationNumber = std::stoi(timeBoundaries[2]);
    std::cout << "Simulation number: " << simulationNumber << std::endl;

    double tStart = std::strtod(timeBoundaries[0], 0);
    double tEnd   = std::strtod(timeBoundaries[1], 0);
    std::cout << "Simulation time t=[" << tStart << ", " << tEnd << "]" << std::endl;
    return std::make_pair(tStart, tEnd);
}

void getAndPrintNetworkParameters(size_t &nPopulation, size_t &nEdges, int &maxContactsA, int &maxContactsB,
                                  double &newContRate, double &looseContRate,
                                  char* arr[])
{
    nPopulation = std::stoi(arr[0]);
    std::cout << "Population size: " << nPopulation << std::endl;

    nEdges = std::stoi(arr[1]);
    std::cout << "Initial edges: " << nEdges << std::endl;

    maxContactsA = nPopulation - 1;
    maxContactsB = nPopulation - 1;
    std::cout << "Max. contact boundaries =[" << maxContactsA << ", " << maxContactsB << "]" << std::endl;

    newContRate = std::strtod(arr[2], 0);
    std::cout << "New Contact Rate: " << newContRate << std::endl;

    looseContRate = std::strtod(arr[3], 0);
    std::cout << "Loose Contact Rate: " << looseContRate << std::endl;
}

void viralDynamics(ConfigurationFile const& )//config)
{
}

void viralDynamics(int argc, char* argv[])
{
    if (argc != 13)
    {
        std::string msg = "Invalid parameters 2";
        throw std::domain_error(msg);
    }
    else
    {

        char* arr[] = {argv[2], argv[3], argv[12]};
        size_t simulationNumber = 0;
        std::pair<double, double> simulationTime = getAndPrintSimulationParameters(arr, simulationNumber);

        size_t nPopulation = 0;
        size_t nEdges = 0;
        int maxContactsA = 0;
        int maxContactsB = 0;
        double newContRate = 0;
        double looseContRate = 0;

        char* arr1[]  = {argv[4], argv[6], argv[7], argv[8],};
        getAndPrintNetworkParameters(nPopulation, nEdges, maxContactsA, maxContactsB,
                                     newContRate, looseContRate, arr1);


        size_t nInfected = std::stoi(argv[5]);
        std::cout << "Number of infected: " << nInfected << std::endl;


        double diagnosisRate = std::strtod(argv[9], 0);
        std::cout << "Diagnosis rate: " << diagnosisRate << std::endl;

        double deathRate = std::strtod(argv[10], 0);
        std::cout << "Death rate: " << deathRate << std::endl;

        double transmitRate = std::strtod(argv[11], 0);
        std::cout << "Transmission rate: " << transmitRate << std::endl;

        double birthRate = 0;

        executeSSA(simulationTime.first, simulationTime.second, nInfected, nPopulation - nInfected, nEdges, maxContactsA, maxContactsB,
                   transmitRate, newContRate, looseContRate, diagnosisRate, deathRate, birthRate, simulationNumber);

    }

}
