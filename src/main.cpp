#include <algorithm/Algorithm.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <network/Settings.hpp>
#include <utils/Json.hpp>

#include <chrono>
#include <iostream>
#include <fstream>


int main(int argc, char** argv) // NOLINT
{
    if (argc != 2)
    {
        std::cerr << "No configuration file provided\n";
        return -1;
    }

    try
    {
        auto const start_time = std::chrono::system_clock::now().time_since_epoch().count();
        auto file = std::ifstream{argv[1]}; // NOLINT
        auto const config = configuration::Configuration{file};
        
        auto json = utils::json::Block{};

        auto network = network::ContactNetwork{config};
        auto algo = algorithm::choose_algorithm(config, network);

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

        auto out_file = std::ofstream{std::to_string(start_time) + ".json"};
        out_file << json.to_string();

    }
    catch (std::exception const& e)
    {
        std::cerr << "Error while executing: " << e.what() << '\n';
        return -2;
    }

    return 0;
}

