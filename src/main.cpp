#include <algorithm/SSA.hpp>
#include <configuration/Configuration.hpp>
#include <network/ContactNetwork.hpp>
#include <settings/Settings.hpp>

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

        auto network = network::ContactNetwork{settings.network()};
        auto ssa = algorithm::SSA{settings.algorithm(), network};
        ssa.run();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error while executing: " << e.what() << '\n';
        return -2;
    }

    return 0;
}

