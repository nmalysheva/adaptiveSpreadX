#ifndef TYPES_DISTRIBUTION_HPP_
#define TYPES_DISTRIBUTION_HPP_

#include <functional>


class Distribution final
{
  public:
    using value_type = double;

    Distribution(char const distribution, value_type const a, value_type const b);

    explicit Distribution(value_type const value);

    auto draw() const -> value_type;
    auto draw(unsigned const count) const -> value_type;

    [[nodiscard]]
    auto operator()() const -> value_type;

  private:
    std::function<value_type(void)> m_draw{};
};

#endif

