#include <catch.hpp>

#include <utils/Algorithm.hpp>

#include <vector>


TEST_CASE("algorithm_only_positive_value_added")
{
    auto result = std::vector<int>{};
    
    utils::emplace_if_positive(result, -1);
    REQUIRE(result.empty());

    utils::emplace_if_positive(result, 0);
    REQUIRE(result.empty());
    
    utils::emplace_if_positive(result, 10);
    REQUIRE(result.size() == 1);
    REQUIRE(result.front() == 10);
}


TEST_CASE("algorithm_repeat_n")
{
    auto i = 0;

    utils::repeat_n(5, [&i](){ ++i; });

    REQUIRE(i == 5);
}

