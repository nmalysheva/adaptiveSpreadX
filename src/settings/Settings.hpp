#ifndef SETTINGS_SETTINGS_HPP_
#define SETTINGS_SETTINGS_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "Algorithm.hpp"
#include "Network.hpp"


namespace settings
{

class Settings final
{
  public:
    Settings(std::unordered_map<std::string, std::vector<std::string>> const& data);

    auto network() const noexcept -> Network const&;

    auto algorithm() const noexcept -> Algorithm const&;

  private:
    Network m_network{};
    Algorithm m_algorithm{};
};

} // namespace settings

#endif

