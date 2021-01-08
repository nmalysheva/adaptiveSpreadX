#ifndef CONFIGURATION_SPECIESCONFIGURATION_HPP_
#define CONFIGURATION_SPECIESCONFIGURATION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <configuration/IConfiguration.hpp>
#include <configuration/ConfigurationBlock.hpp>
#include <types/Distribution.hpp>

#include <string>
#include <tuple>
#include <vector>


class SpeciesConfiguration final : public IConfiguration
{
  public:
    using data_type = std::tuple<std::string, Distribution, Distribution>;

    auto add(std::string_view line) -> void override;

    auto get() const noexcept -> std::vector<data_type> const&;

  private:
    std::vector<data_type> m_data;
};

#endif

