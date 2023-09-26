#ifndef TYPES_PROPABILITY_HPP_
#define TYPES_PROPABILITY_HPP_

/*!
 * \brief A propability is a float value in the range [0.0, 1.0].
 *
 * This class is mainly used to allow checked reading from the configuration.
 */
class Propability final
{
  public:
    explicit Propability(double val);

    explicit operator double() const noexcept;

  private:
    double const m_value{0.0};
};

#endif

