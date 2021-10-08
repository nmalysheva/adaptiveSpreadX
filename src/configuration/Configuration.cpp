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

    auto it = add_header(helper::unheader(stream.get_line()));
   
    while (stream.has_next_line())
    {
        auto const& line = stream.get_line();
        switch (helper::classify(line))
        {
            case helper::Category::comment:
                break;

            case helper::Category::header:
                it = add_header(helper::unheader(line));
                break;

            case helper::Category::data:
                [[fallthrough]];

            default:
                it->second.push_back(line);
                break;
        }
    }
}
    
auto Configuration::get(std::string name) const -> std::optional<std::reference_wrapper<entry_list const>>
{
    auto const it = m_data.find(name);
    if (it == m_data.end())
    {
        return std::nullopt;
    }

    m_used_headers.emplace(std::move(name));

    return std::cref(it->second);
}


auto Configuration::add_header(std::string&& name) -> storage_type::iterator
{
    auto [it, inserted] = m_data.try_emplace(name, storage_type::mapped_type{});
    if (not inserted)
    {
        throw Exception{error::HeaderAlreadyUsed};
    }

    return it;
}


auto Configuration::get_unused() const noexcept -> std::optional<std::string>
{
    for (auto const& entry : m_data)
    {
        if (m_used_headers.count(entry.first) == 0)
        {
            return entry.first;
        }
    }

    return std::nullopt;
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

