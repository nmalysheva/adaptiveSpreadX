#ifndef TYPES_UNSIGNED_HPP_
#define TYPES_UNSIGNED_HPP_

#include <cstddef>
#include <string>


class Unsigned final
{
  public:
    explicit Unsigned(std::string const&)
        : m_data{0}
    {
    }

  private:
    std::size_t m_data;
};

#endif

