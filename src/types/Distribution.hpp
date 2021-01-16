#ifndef TYPES_DISTRIBUTION_HPP_
#define TYPES_DISTRIBUTION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <types/Propability.hpp>

#include <string>

#include <functional>


class Distribution final
{
  public:
    Distribution() = delete;

    Distribution(std::string const&)
    {}

    Distribution(char const distribution, Propability const a, Propability const b);

    explicit Distribution(Propability const value);

    [[nodiscard]]
    auto operator()() const -> Propability;

  private:
    std::function<Propability(void)> m_draw{};
};


#endif

