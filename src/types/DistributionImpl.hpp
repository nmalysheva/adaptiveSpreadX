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
 * - Poisson
 * - Binomial
 */
class DistributionImpl
{
  public:
    /// draw a random value from the implemented distribution
    [[nodiscard]]
    virtual auto draw() -> double = 0;

    /// return the maximum possible value of draw()
    [[nodiscard]]
    virtual auto max() const -> double = 0;

  protected:
    /// random number engine
    std::mt19937 m_generator{std::random_device{}()};
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
     * \param value the value of this "distribution"
     */
    FixedDistribution(double value);

    /// \copydoc DistributionImpl::draw
    [[nodiscard]]
    auto draw() -> double override;

    /// \copydoc DistributionImpl::max
    [[nodiscard]]
    auto max() const -> double override;

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
     * \throws std::invalid_argument if a < 0
     *
     * \param a minimum of the distribution
     * \param b maximum of the distribution
     */
    UniformDistribution(double a, double b);
    
    /// \copydoc DistributionImpl::draw
    [[nodiscard]]
    auto draw() -> double override;
    
    /// \copydoc DistributionImpl::max
    [[nodiscard]]
    auto max() const -> double override;

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
     * \param m mean
     * \param s standard deviation
     */
    NormalDistribution(double m, double s);
    
    /// \copydoc DistributionImpl::draw
    /// Redraws if the value is out of range
    [[nodiscard]]
    auto draw() -> double override;

    /// \copydoc DistributionImpl::max
    [[nodiscard]]
    auto max() const -> double override;

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
    [[nodiscard]]
    auto draw() -> double override;

    /// \copydoc DistributionImpl::max
    [[nodiscard]]
    auto max() const -> double override;

  private:
    /// the underlaying distribution
    std::exponential_distribution<> m_dist;
};


/// Implements a poisson distribution
class PoissonDistribution final : public DistributionImpl
{
  public:
    /*!
     * \brief Create a poisson distribution.
     *
     * Values from P(l) will be drawn.
     *
     * \param l Lambda, the mean of the distribution
     */
    PoissonDistribution(double l);
    
    /// \copydoc DistributionImpl::draw
    [[nodiscard]]
    auto draw() -> double override;

    /// \copydoc DistributionImpl::max
    [[nodiscard]]
    auto max() const -> double override;

  private:
    /// the underlaying distribution
    std::poisson_distribution<> m_dist;
};


/// Implements a binomial distribution
class BinomialDistribution final : public DistributionImpl
{
  public:
    /*!
     * \brief Create a binomial distribution.
     *
     * Values from B(n, p) will be drawn.
     *
     * \param n number of trials
     * \param p propability for success
     */
    BinomialDistribution(unsigned n, double p);
    
    /// \copydoc DistributionImpl::draw
    [[nodiscard]]
    auto draw() -> double override;

    /// \copydoc DistributionImpl::max
    [[nodiscard]]
    auto max() const -> double override;

  private:
    /// the underlaying distribution
    std::binomial_distribution<unsigned> m_dist;
};

#endif

