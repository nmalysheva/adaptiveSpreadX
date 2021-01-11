#ifndef CONFIGURATION_TRANSITIONCONFIGURATION_HPP_
#define CONFIGURATION_TRANSITIONIONCONFIGURATION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <types/Distribution.hpp>

#include <unordered_map>
#include <string>


class TransitionConfiguration final
{
  public:
    using DataType = std::unordered_map<std::string, Distribution>;

    auto add(std::string_view line) -> void;

    auto get() const noexcept -> std::unordered_map<std::string, DataType> const&;

  private:
    std::unordered_map<std::string, DataType> m_data;
};

#endif

