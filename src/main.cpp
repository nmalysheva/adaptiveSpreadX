#include <algorithm/Algorithm.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <network/Settings.hpp>
#include <utils/Json.hpp>

#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <unistd.h>


/// Generate filename: yymmdd_hhmmss_pid.json
auto make_filename() -> std::string;


int main(int argc, char** argv) // NOLINT
{
    if (argc != 2)
    {
        std::cerr << "No configuration file provided\n";
        return -1;
    }

    try
    {
        auto const filename = make_filename();
        
        auto file = std::ifstream{argv[1]}; // NOLINT
        auto const config = configuration::Configuration{file};
        
        auto json = utils::json::Block{};

        auto network = network::ContactNetwork{config};
        auto algo = algorithm::make_algorithm(config, network);

        if (auto const unused = config.get_unused(); unused)
        {
            throw std::logic_error{std::string{"Unknown section: "} + *unused};
        }

        json.add_json("configuration", config.to_json());

        auto const start = std::chrono::system_clock::now();
        algo->run(json);
        auto const end = std::chrono::system_clock::now();
        auto const duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
        json.add_number("runtime", duration);

        auto out_file = std::ofstream{filename};
        out_file << json.to_string();

    }
    catch (std::exception const& e)
    {
        std::cerr << "Error while executing: " << e.what() << '\n';
        return -2;
    }

    return 0;
}


auto make_filename() -> std::string
{
    using std::chrono::system_clock;
    auto const start_time = system_clock::to_time_t(system_clock::now());
    constexpr auto num_chars = 14;
    auto result = std::string(num_chars, 'x');
    std::strftime(result.data(), result.size(), "%y%m%d_%H%M%S", std::localtime(&start_time));
    result.back() = '_'; // overwrite the 0-byte
    result += std::to_string(getpid()) + ".json";
    return result;
}

