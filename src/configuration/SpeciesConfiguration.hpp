#ifndef CONFIGURATION_SPECIESCONFIGURATION_HPP_
#define CONFIGURATION_SPECIESCONFIGURATION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <configuration/IConfiguration.hpp>
#include <types/Distribution.hpp>

#include <map>
#include <string>
#include <utility>


class SpeciesConfiguration final : public IConfiguration
{
  public:
    using DistributionPair = std::pair<Distribution, Distribution>;

    auto add(std::string_view line) -> void override;

    auto get() const noexcept -> std::unordered_map<std::string, DistributionPair> const&;

  private:
    std::unordered_map<std::string, DistributionPair> m_data;
};

#endif

