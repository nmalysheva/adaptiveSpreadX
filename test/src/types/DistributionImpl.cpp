#include <catch.hpp>

#include <types/DistributionImpl.hpp>

#include <limits>


TEST_CASE("distributionimpl_fixed_ok")
{
    try
    {
        const auto val = 0.1;
        auto d = FixedDistribution{val};
        REQUIRE(d.draw() == val);
        REQUIRE(d.max() == val);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distributionimpl_fixed_outofrange")
{
    REQUIRE_THROWS(FixedDistribution{-1.0});
}


TEST_CASE("distributionimpl_uniform_ok")
{
    try
    {
        auto const a = 0.0;
        auto const b = 0.2;
        auto d = UniformDistribution{a, b};
        auto const v = d.draw();
        REQUIRE(std::clamp(v, a, b) == v);
        REQUIRE(d.max() == b);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distributionimpl_uniform_fail_outofrange")
{
    REQUIRE_THROWS(UniformDistribution{-1.0, 0.1});
}


TEST_CASE("distributionimpl_uniform_fail_minmax")
{
    REQUIRE_THROWS(UniformDistribution{0.9, 0.8});
}


TEST_CASE("distributionimpl_normal_ok")
{
    try
    {
        auto d = NormalDistribution{-0.5, 1.0};
        auto const v = d.draw();
        REQUIRE(v >= 0.0);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distributionimpl_exponential_ok")
{
    try
    {
        auto d = ExponentialDistribution{0.1};
        auto const v = d.draw();
        REQUIRE(v >= 0.0);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distributionimpl_poisson_ok")
{
    try
    {
        const auto l = 2.0;
        auto d = PoissonDistribution{l};
        REQUIRE(d.draw() >= 0.0);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distributionimpl_poisson_not_positive")
{
    REQUIRE_THROWS(PoissonDistribution{0});
    REQUIRE_THROWS(PoissonDistribution{-0.1});
}


TEST_CASE("distributionimpl_binomial_ok")
{
    try
    {
        const auto n = 10;
        const auto p = 0.1;
        auto d = BinomialDistribution{n, p};
        REQUIRE(d.draw() >= 0.0);
        REQUIRE(d.max() == n);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distributionimpl_binomial_p_negative")
{
    REQUIRE_THROWS(BinomialDistribution{1, -1.0});
}


TEST_CASE("distributionimpl_binomial_p_greater_1")
{
    REQUIRE_THROWS(BinomialDistribution{1, 1.1});
}
