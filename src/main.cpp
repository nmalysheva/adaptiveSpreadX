#include <algorithm/SSA.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <settings/Settings.hpp>
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
        auto file = std::ifstream{argv[1]}; // NOLINT
        auto const config = configuration::Configuration{file};
        auto const settings = settings::Settings{config.get()};

        auto json = utils::json::Block{};

        auto network = network::ContactNetwork{settings.network()};
        auto ssa = algorithm::SSA{settings.algorithm(), network};

        json.add_json("configuration", config.to_json());

        auto const start = std::chrono::system_clock::now();
        ssa.run(json);
        auto const end = std::chrono::system_clock::now();
        auto const duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
        json.add_number("runtime", duration);

        //this should go to a file
        std::cout << json.to_string() << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error while executing: " << e.what() << '\n';
        return -2;
    }

    return 0;
}

