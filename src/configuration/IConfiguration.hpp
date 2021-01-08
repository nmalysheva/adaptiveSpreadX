#ifndef CONFIGURATION_ICONFIGURATION_HPP_
#define CONFIGURATION_ICONFIGURATION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <string_view>

/*!
 * \brief Interface for configurations.
 *
 * Each configuration should implement this interface
 * to unify initialisation.
 */
class IConfiguration
{
  public:
    /// Add a configuration entry (e.g. line from a file) to this configuration.
    virtual auto add(std::string_view line) -> void = 0;
};

#endif

