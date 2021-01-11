#include <configuration/ConfigurationFile.hpp>

#include <configuration/ConfigurationLine.hpp>

#include <utils/String.hpp>

#include <fstream>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <tuple>

namespace
{
template <typename T>
static auto add_config(T& rules, std::string_view str) -> void
{
    auto val = std::make_from_tuple<typename T::value_type>(parse<>(str, typename T::value_type::ParserTypes{}));
    if (rules.exists(val))
    {
        throw std::invalid_argument{"entry already set"};
    }
    rules.put(std::move(val));
}
}


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

    auto const config_builder = std::unordered_map<std::string, std::function<void(std::string_view)>> {
        {"", [](std::string_view) { throw std::invalid_argument{"no section set"}; }},
            {"[Edges]", [this](std::string_view str) { this->m_edges.add(str); }},
            {"[Network]", [this](std::string_view str) { add_config(this->m_network, str); }},
            {"[Species]", [this](std::string_view str) { add_config(this->m_species, str); }},
            //{"[Deaths]", [this](std::string_view str) { add_config(this->m_deaths, str); }},
            //{"[Births]", [this](std::string_view str) { add_config(this->m_births, str); }},
            //{"[Interactions]", [this](std::string_view str) { this->m_interactions.add(str); }},
            //{"[Transitions]", [this](std::string_view str) { this->m_transitions.add(str); }},
    };

    auto section = config_builder.begin();

    while (input.good())
    {
        auto raw = std::string{};
        std::getline(input, raw);
        auto const line = ConfigurationLine{std::move(raw)};

        if (line.content().empty())
        {
            continue;
        }

        if (line.is_header())
        {
            section = config_builder.find(line.content()); 
            if (section == config_builder.end())
            {
                throw std::invalid_argument{std::string{"Unknown configuration: "} + line.content()};
            }
        }
        else
        {
            try
            {
                section->second(line.content());
            }
            catch (std::exception const& e)
            {
                auto msg = std::stringstream{};
                msg << "Error while parsing section " << section->first << ": '" << line.content() << "\'\n\t" << e.what();
                throw std::invalid_argument{msg.str()};
            }
        }
    }
}


auto ConfigurationFile::get_edges() const -> EdgeConfiguration const&
{
    return m_edges;
}


auto ConfigurationFile::get_network() const -> NodeInitilisations const&
{
    return m_network;
}


auto ConfigurationFile::get_species() const -> SpeciesFactories const&
{
    return m_species;
}


auto ConfigurationFile::get_deaths() const -> Transformations const&
{
    return m_deaths;
}


auto ConfigurationFile::get_births() const -> Transformations const&
{
    return m_births;
}

auto ConfigurationFile::get_interactions() const -> InteractionConfiguration const&
{
    return m_interactions;
}

auto ConfigurationFile::get_transitions() const -> TransitionConfiguration const&
{
    return m_transitions;
}
