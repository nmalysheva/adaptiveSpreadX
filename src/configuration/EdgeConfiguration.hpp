#ifndef CONFIGURATION_EDGECONFIGURATION_HPP_
#define CONFIGURATION_EDGECONFIGURATION_HPP_

#include <cstddef>
#include <optional>


class EdgeConfiguration final
{
  public:
    auto add(std::string_view line) -> void;
    auto get() const noexcept -> std::size_t;

  private:
    std::optional<std::size_t> m_data{};
};

#endif

