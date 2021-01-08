#include <configuration/ConfigurationFile.hpp>

#include <configuration/ConfigurationLine.hpp>
#include <utils/String.hpp>

#include <fstream>
#include <memory>
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

    auto block = std::string{};
    IConfiguration* config{nullptr};

    while (input.good())
    {
        auto raw = std::string{};
        std::getline(input, raw);
        auto const line = ConfigurationLine{trim(raw)};

        if (line.content().empty())
        {
            continue;
        }

        if (line.is_header())
        {
            if (line.content() == "[Species]")
            {
                config = std::addressof(m_species);
            }
            else if (line.content() == "[Network]")
            {
                config = std::addressof(m_network);
            }
            else
            {
                throw std::invalid_argument{std::string{"Unknown configuration: "} + line.content()};
            }
        }
        else if (config)
        {
            config->add(line.content());
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


auto ConfigurationFile::get_network() const -> NetworkConfiguration const&
{
    return m_network;
}


auto ConfigurationFile::get_species() const -> SpeciesConfiguration const&
{
    return m_species;
}

