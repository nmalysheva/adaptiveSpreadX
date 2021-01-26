#include "Configuration.hpp"
#include "Exception.hpp"
#include "Helper.hpp"
#include "Stream.hpp"


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

} // namespace configuration

