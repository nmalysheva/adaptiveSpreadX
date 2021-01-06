#include "ConfigurationFile.hpp"

#include <utils/String.hpp>

#include <fstream>
#include <stdexcept>
#include <utility>


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

    if (input.eof())
    {
        auto const msg = std::string{"ConfigurationFile: file is empty "} + path.data();
        throw std::runtime_error{msg};
    }

    auto line = std::string{};
    auto block = std::string{};
    while (input.good())
    {
        std::getline(input, line);
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        switch (line.front())
        {
            case ':':
                block = std::move(line);
                break;
            case '#':
                break;
            default:
                if (not block.empty())
                {
                    m_data[block].emplace_back(std::move(line));
                }
                break;
        }
    }
}


auto ConfigurationFile::get_config(std::string const& name) const -> ConfigurationBlock const&
{
    auto const it = m_data.find(name);
    if (it == m_data.cend())
    {
        static auto empty = ConfigurationBlock{};
        return empty;
    }

    return it->second;
}

