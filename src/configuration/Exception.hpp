#ifndef CONFIGURATION_EXCEPTION_HPP_
#define CONFIGURATION_EXCEPTION_HPP_

#include <exception>


namespace configuration
{

/// descriptions of possible `configuration::Exception`s
namespace error
{
/// Stream is not readable.
static auto const NotReadable = "stream not readable";

/// section has no header 
static auto const NoHeader = "configuration without section";

/// section header is empty
static auto const EmptyHeader = "empty header provided";

/// section header used a second time
static auto const HeaderAlreadyUsed = "each header must only be used once";

/// no section with given header available
static auto const UnknownSection = "no section found";
}

/// custom exception for errors while processing the configuration
class Exception final : public std::exception
{
  public:
    /// Create object with given message.
    Exception(char const* const msg) noexcept;

    /// Get pointer to the error message.
    auto what() const noexcept -> char const*;

  private:
    /// Pointer to the error message.
    char const* const m_msg;
};

} // namespace configuration

#endif

