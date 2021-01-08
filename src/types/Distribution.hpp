#ifndef TYPES_DISTRIBUTION_HPP_
#define TYPES_DISTRIBUTION_HPP_

/*! \file
 * \author Mathias Lindemann
 */

#include <types/Propability.hpp>

#include <functional>


class Distribution final
{
  public:
    Distribution() = delete;

    Distribution(char const distribution, Propability const a, Propability const b);

    explicit Distribution(Propability const value);

    [[nodiscard]]
    auto draw() const -> Propability;

  private:
    std::function<Propability(void)> m_draw{};
};


#endif

