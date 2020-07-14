//
// Created by Malysheva, Nadezhda on 10.07.20.
//
#include "Utility.h"
#include <fstream>


size_t binarySearch(std::vector<std::pair<double, lemon::ListGraph::Edge>> propCumSum,
                                         size_t indL, size_t indR, double rStart, double rBound)
{
    size_t result = size_t(-1);

    if (indR == indL)
    {
        if (propCumSum.at(indR).first + rStart  >= rBound)
        {
            result = indR;
            //propCumSum.erase(propCumSum.begin() + indR);
        }
        return result;

    }
    else if (indR >= indL)
    {
        int mid = indL + (indR - indL) / 2;

        // If the element is present at the middle
        // itself
        if (propCumSum.at(mid).first + rStart < rBound)
        {
            return binarySearch(propCumSum, mid + 1, indR, rStart, rBound);
        }

            // If element is smaller than mid, then
            // it can only be present in left subarray
        else
        {
            return binarySearch(propCumSum, indL, mid, rStart, rBound);
        }

        // Else the element can only be present
        // in right subarray
        //return binarySearch(propCumSum, mid + 1, indR, rBound);
    }

    // We reach here when element is not
    // present in array
    //return result;
}


double sampleRandUni(std::mt19937_64 generator)
{
    std::uniform_real_distribution<> randuni;
    double r = randuni(generator);
    while (r == 0)
    {
        r = randuni(generator);
    }
    return r;
}

void printBenFile(std::string fileName, const std::vector<BenStructure> &benToFile)
{
    std::ofstream benFile;
    benFile.open(fileName);
    for (auto &it: benToFile)
    {
        std::string stateStr = "False";
        if (it.state)
        {
            stateStr = "True";
        }
        benFile << it.t << " " << it.u << " " << it.v << " " << stateStr << std::endl;
    }
    benFile.close();

}
