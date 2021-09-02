#ifndef TYPES_DISTRIBUTIONIMPL_HPP_
#define TYPES_DISTRIBUTIONIMPL_HPP_

#include <random>
#include <vector>


/*!
 * \brief Base class for a distribution implemention.
 *
 * Possible implementations are
 * - Fixed value
 * - Uniform
 * - Normal
 * - Exponential
 */
class DistributionImpl
{
  public:
    /// draw a random value from the implemented distribution
    virtual auto draw() -> double = 0;
   
    /// random number engine (same for all distributions)
    static std::mt19937 Generator;
};


/*!
 * \brief Implements a fixed value "distribution".
 *
 * Not a real distribution, but returns the same value everytime.
 */
class FixedDistribution final : public DistributionImpl
{
  public:
    /*!
     * \brief Create a fixed distribution, that returns the given `value` on every call to `draw`.
     *
     * \param value The value of this "distribution"
     */
    FixedDistribution(double value);

    /// \copydoc DistributionImpl::draw
    auto draw() -> double override;

  private:
    /// the value to return
    double const m_value;
};


/// Implements a uniform distribution
class UniformDistribution final : public DistributionImpl
{
  public:
    /*!
     * \brief Create a uniform distribution.
     *
     * Values from the range [a,b] are drawn.
     *
     * \throws std::invalid_argument if b < a
     * \throws std::invalid_argument if a < 0 or b > 1
     *
     * \param a minimum of the distribution
     * \param b maximum of the distribution
     */
    UniformDistribution(double a, double b);
    
    /// \copydoc DistributionImpl::draw
    auto draw() -> double override;

  private:
    /// the underlaying distribution
    std::uniform_real_distribution<> m_dist;
};


/// Implements a normal distribution
class NormalDistribution final : public DistributionImpl
{
  public:
    /*!
     * \brief Create a normal distribution.
     *
     * Values from N(m,s) will be drawn.
     *
     * \throws std::invalid_argument if less than 50% of the values are not in range [0,1].
     *
     * \param m Mean
     * \param s Standard deviation
     */
    NormalDistribution(double m, double s);
    
    /// \copydoc DistributionImpl::draw
    auto draw() -> double override;

  private:
    /// the underlaying distribution
    std::normal_distribution<> m_dist;
};


/// Implements an exponential distribution
class ExponentialDistribution final : public DistributionImpl
{
  public:
    /*!
     * \brief Create an exponential distribution.
     *
     * Values from E(l) will be drawn.
     *
     * \param l Lambda, the rate of the distribution
     */
    ExponentialDistribution(double l);
    
    /// \copydoc DistributionImpl::draw
    auto draw() -> double override;

  private:
    /// the underlaying distribution
    std::exponential_distribution<> m_dist;
};

#endif

