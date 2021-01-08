#ifndef CONFIGURATION_NETWORKCONFIGURATION_HPP_
#define CONFIGURATION_NETWORKCONFIGURATION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <configuration/IConfiguration.hpp>
#include <types/Distribution.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>


class NetworkConfiguration final : public IConfiguration
{
  public:
    auto add(std::string_view line) -> void override;

    auto get_nodes() const noexcept -> std::unordered_map<std::string, std::size_t> const&;

    auto num_edges() const noexcept -> std::size_t;

  private:
    std::optional<std::size_t> m_number_of_edges{};
    std::unordered_map<std::string, std::size_t> m_data{};
};

#endif

