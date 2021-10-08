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

/// configuration contains no sections
static auto const NoSections = "no sections defined";

/// section has no header 
static auto const NoHeader = "configuration without section";

/// section header is empty
static auto const EmptyHeader = "empty header provided";

/// section header used a second time
static auto const HeaderAlreadyUsed = "each header must only be used once";

/// section unknown
static auto const UnknownSection = "unknown section requested";
}

/// custom exception for errors while processing the configuration
class Exception final : public std::exception
{
  public:
    /// Create object with given message.
    Exception(char const* msg) noexcept;

    /// Get pointer to the error message.
    [[nodiscard]]
    auto what() const noexcept -> char const* override;

  private:
    /// Pointer to the error message.
    char const* const m_msg;
};

} // namespace configuration

#endif

