#ifndef CONFIGURATION_INTERACTIONCONFIGURATION_HPP_
#define CONFIGURATION_INTERACTIONIONCONFIGURATION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <types/Distribution.hpp>

#include <unordered_map>
#include <string>
#include <utility>


class InteractionConfiguration final
{
  public:
    using DataType = std::unordered_map<std::string, std::pair<std::string, Distribution>>;

    auto add(std::string_view line) -> void;

    auto get() const noexcept -> std::unordered_map<std::string, DataType> const&;

  private:
    std::unordered_map<std::string, DataType> m_data;
};

#endif

