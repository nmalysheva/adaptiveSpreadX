#include <algorithm/SSA.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <settings/Settings.hpp>
#include <utils/Json.hpp>

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

        // measure time
        ssa.run(json);
        // end time

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

