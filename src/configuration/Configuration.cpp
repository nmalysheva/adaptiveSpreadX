#include "Configuration.hpp"
#include "Exception.hpp"
#include "Helper.hpp"
#include "Stream.hpp"

#include <utils/Json.hpp>


namespace configuration
{

Configuration::Configuration(std::istream& input)
{
    auto stream = Stream{input};

    if (not helper::find_first_header(stream))
    {
        throw Exception{error::NoSections};
    }
    
    auto* block = insert_header(helper::unheader(stream.get_line()));
   
    while (stream.has_next_line())
    {
        auto const& line = stream.get_line();
        switch (helper::classify(line))
        {
            case helper::Category::comment:
                break;

            case helper::Category::header:
                block = insert_header(helper::unheader(line));
                break;

            case helper::Category::data:
                [[fallthrough]];

            default:
                block->push_back(line);
                break;
        }
    }
}


auto Configuration::insert_header(std::string const& str) -> std::vector<std::string>*
{
    auto [it, success] = m_data.try_emplace(str, std::vector<std::string>{});
    if (not success)
    {
        throw Exception{error::HeaderAlreadyUsed};
    }

    return &it->second;
}


auto Configuration::to_json() const -> std::string
{
    auto json = utils::json::Block{};

    for (auto const& [name, config] : m_data)
    {
        auto list = utils::json::List<std::string>{};
        std::for_each(config.cbegin(), config.cend(), [&list](auto const& data) { list.add(std::string{'"'} + data + '"'); });
        json.add_json(name, list.to_string());
    }

    return json.to_string();
}

} // namespace configuration

