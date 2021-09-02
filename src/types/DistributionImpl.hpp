#ifndef TYPES_DISTRIBUTIONIMPL_HPP_
#define TYPES_DISTRIBUTIONIMPL_HPP_

#include <random>
#include <vector>


class DistributionImpl
{
  public:
    virtual auto draw() -> double = 0;
   
    /// random number engine (same for all distributions)
    static std::mt19937 Generator;
};


class FixedDistribution final : public DistributionImpl
{
  public:
    FixedDistribution(double value);
    auto draw() -> double override;

  private:
    double const m_value;
};



class UniformDistribution final : public DistributionImpl
{
  public:
    UniformDistribution(double a, double b);
    auto draw() -> double override;

  private:
    std::uniform_real_distribution<> m_dist;
};


class NormalDistribution final : public DistributionImpl
{
  public:
    NormalDistribution(double m, double s);
    auto draw() -> double override;

  private:
    std::normal_distribution<> m_dist;
};

class ExponentialDistribution final : public DistributionImpl
{
  public:
    ExponentialDistribution(double l);
    auto draw() -> double override;

  private:
    std::exponential_distribution<> m_dist;
};

#endif

