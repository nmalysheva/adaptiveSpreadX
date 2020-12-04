//
// Created by Malysheva, Nadezhda on 10.07.20.
//
#include "Utility.h"

#include <limits>


double sampleRandUni(std::mt19937_64 &generator)
{
    std::uniform_real_distribution<> randuni{std::numeric_limits<double>::min()};
    return randuni(generator);
}

