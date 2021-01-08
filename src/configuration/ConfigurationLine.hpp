#ifndef CONFIGURATION_CONFIGLINE_HPP_
#define CONFIGURATION_CONFIGLINE_HPP_

#include <string>


class ConfigurationLine final
{
  public:
    ConfigurationLine() = delete;

    ConfigurationLine(std::string&& line) noexcept;

    auto is_header() const noexcept -> bool;

    auto content() const noexcept -> std::string const&;

  private:
    std::string const m_line;

    static auto check(std::string&& str) noexcept -> std::string;
};

#endif

