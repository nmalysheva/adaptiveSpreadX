#include <catch.hpp>

#include <algorithm/Settings.hpp>

#include <fstream>

using namespace algorithm;


auto make_settings(std::string const& name)
{
    auto const path = std::string{TEST_CONFIG_FOLDER} + "/algorithm/" + name;
    auto fs = std::ifstream{path};
    auto const config = configuration::Configuration{fs};
    return Settings{config};
}


TEST_CASE("settings_algo_missing")
{
    REQUIRE_THROWS_WITH(make_settings("algo_missing.txt"), Settings::AlgorithmMissing);
}


TEST_CASE("settings_use_algorithm")
{
    auto const a = make_settings("ssa.txt");
    REQUIRE(a.algorithm() == Settings::Algorithm::SSA);

    auto const b = make_settings("ssatan-x.txt");
    REQUIRE(b.algorithm() == Settings::Algorithm::SSATANX);

    auto const c = make_settings("default.txt");
    REQUIRE(c.algorithm() == Settings::DefaultAlgorithm);

    REQUIRE_THROWS_WITH(make_settings("unknown_algorithm.txt"), Settings::UnknownAlgorithm);
    REQUIRE_THROWS_WITH(make_settings("duplicate_algorithm.txt"), Settings::DuplicateAlgorithm);
}


TEST_CASE("settings_time")
{
    auto const a = make_settings("ssa.txt");
    REQUIRE(a.time() == 123);

    auto const b = make_settings("ssatan-x.txt");
    REQUIRE_THROWS_WITH(b.time(), Settings::TimeMissing);

    REQUIRE_THROWS_WITH(make_settings("time_twice.txt"), Settings::DuplicateTime);
}


TEST_CASE("settings_epsilon")
{
    auto const a = make_settings("ssatan-x.txt");
    REQUIRE(a.epsilon() == 0.1);

    auto const b = make_settings("ssa.txt");
    REQUIRE(b.epsilon() == Settings::DefaultEpsilon);

    REQUIRE_THROWS_WITH(make_settings("epsilon_twice.txt"), Settings::DuplicateEpsilon);
}


TEST_CASE("settings_output")
{
    auto const a = make_settings("ssa.txt");
    REQUIRE(a.output_step() == 100);

    auto const b = make_settings("ssatan-x.txt");
    REQUIRE(b.output_step() == Settings::DefaultOutput);

    REQUIRE_THROWS_WITH(make_settings("output_twice.txt"), Settings::DuplicateOutput);

    auto const c = make_settings("output_too_large.txt");
    REQUIRE(c.output_step() == Settings::DefaultOutput);
}


TEST_CASE("settings_unknown_entry")
{
    REQUIRE_THROWS_WITH(make_settings("unknown_entry.txt"), Settings::UnknownEntry);
}

