#include "ConfigurationFile.hpp"

#include <fstream>
#include <stdexcept>

ConfigurationFile::ConfigurationFile(std::string_view const path)
{
    if (path.empty())
    {
        throw std::runtime_error{"ConfigurationFile: empty path"};
    }

    auto input = std::ifstream{path.data()};
    if (input.fail())
    {
        auto const msg = std::string{"ConfigurationFile: unable to open file "} + path.data();
        throw std::runtime_error{msg};
    }

    if (not input.eof())
    {
        auto const msg = std::string{"ConfigurationFile: file is empty "} + path.data();
        throw std::runtime_error{msg};
    }
}

