#include <catch.hpp>

#include <types/Distribution.hpp>


TEST_CASE("distribution_unknown")
{
    REQUIRE_THROWS(Distribution{'_', {}});
}


TEST_CASE("distribution_constant_value")
{
    try
    {
        auto d = Distribution{0.1};
        REQUIRE(d.draw() == 0.1);
        REQUIRE(d.max() == 0.1);
    }
    catch (...)
    {
        FAIL();
    }
}


TEST_CASE("distribution_uniform")
{
    REQUIRE_NOTHROW(Distribution{'U', {0.0, 1.0}});
}

TEST_CASE("distribution_uniform_fail")
{
    REQUIRE_THROWS(Distribution{'U', {1.0}});
}


TEST_CASE("distribution_normal")
{
    REQUIRE_NOTHROW(Distribution{'N', {0.5, 0.1}});
}

TEST_CASE("distribution_normal_fail")
{
    REQUIRE_THROWS(Distribution{'N', {1.0}});
}


TEST_CASE("distribution_exponential")
{
    REQUIRE_NOTHROW(Distribution{'E', {0.1}});
}

TEST_CASE("distribution_exponenttial_fail")
{
    REQUIRE_THROWS(Distribution{'E', {0.1, 0.2}});
}

