#include <catch.hpp>

#include <types/DistributionImpl.hpp>


TEST_CASE("distributionimpl_fixed_create")
{
    REQUIRE_NOTHROW(FixedDistribution{0.1});
}

TEST_CASE("distributionimpl_fixed_draw")
{
    auto const val = 0.1;
    auto dist = FixedDistribution{val};
    REQUIRE(dist.draw() == val);
}

TEST_CASE("distributionimpl_fixed_outofrange")
{
    REQUIRE_THROWS(FixedDistribution{100.0});
}


TEST_CASE("distributionimpl_uniform_create")
{
    REQUIRE_NOTHROW(UniformDistribution{0.0, 0.2});
}

TEST_CASE("distributionimpl_uniform_draw")
{
    auto const a = 0.0;
    auto const b = 0.2;
    auto d = UniformDistribution{a, b};
    auto const v = d.draw();
    REQUIRE(std::clamp(v, a, b) == v);
}

TEST_CASE("distributionimpl_uniform_fail_outofrange")
{
    REQUIRE_THROWS(UniformDistribution{-1.0, 0.1});
    REQUIRE_THROWS(UniformDistribution{0.2, 5.0});
}

TEST_CASE("distributionimpl_uniform_fail_minmax")
{
    REQUIRE_THROWS(UniformDistribution{0.9, 0.8});
}


TEST_CASE("distributionimpl_normal_create")
{
    REQUIRE_NOTHROW(NormalDistribution{0.5, 0.0001});
}

TEST_CASE("distributionimpl_normal_draw")
{
    auto d = NormalDistribution{0.5, 0.00001};
    auto const v = d.draw();
    REQUIRE(std::clamp(v, 0.0, 1.0) == v);
}

TEST_CASE("distributionimpl_normal_outofrange")
{
    REQUIRE_THROWS(NormalDistribution{0.5, 1.0});
}


TEST_CASE("distributionimpl_exponential_create")
{
    REQUIRE_NOTHROW(ExponentialDistribution{0.1});
}

TEST_CASE("distributionimpl_exponential_draw")
{
    auto d = ExponentialDistribution{0.1};
    auto const v = d.draw();
    REQUIRE(std::clamp(v, 0.0, 1.0) == v);
}

