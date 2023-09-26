#include "Json.hpp"


namespace utils::json
{

auto Block::add_string(std::string const& name, std::string const& content) -> void
{
    auto s = std::stringstream{};
    s << '"' << content << '"';
    add_json(name, s.str());
}


auto Block::add_json(std::string const& name, std::string const& content) -> void
{
    auto s = std::stringstream{};
    s << '"' << name << "\":" << content;
    m_data.emplace_back(s.str());
}

auto Block::to_string() const -> std::string
{
    return std::string{'{'} + implode(m_data) + '}';
}

} // namespace utils::json

