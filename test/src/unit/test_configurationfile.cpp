#include <catch.hpp>
#include <ConfigurationFile.hpp>
#include <ConfigurationFile.cpp>

#include <filesystem>
#include <fstream>

TEST_CASE("no_file_provided")
{
    REQUIRE_THROWS_AS(ConfigurationFile{""}, std::runtime_error);
}

TEST_CASE("file_doesnt_exist")
{
    REQUIRE_THROWS_AS(ConfigurationFile{"noexists"}, std::runtime_error);
}

TEST_CASE("file_is_empty")
{
    constexpr auto name = "empty";
    std::ofstream{name};
    REQUIRE_THROWS_AS(ConfigurationFile{name}, std::runtime_error);
    std::filesystem::remove(name);
}

