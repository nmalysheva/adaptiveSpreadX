//#include <configuration/Config.hpp>
#include <configuration/Configuration.hpp>

#include <network/Settings.hpp>
//#include <ContactNetwork.hpp>
//#include <SSA.hpp>

#include <network/EdgeInitilisation.hpp>
#include <network/NodeInitilisation.hpp>
#include <iostream>
#include <fstream>


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return -1;
    }

    auto file = std::ifstream{argv[1]};
    auto const config = configuration::Configuration{file};
    //auto const configs = configuration::read<network::Settings>(file);
    //auto const ns = std::get<network::Settings>(configs);
//    auto network = ContactNetwork{config};
//    constexpr auto start = 0.0;
//    constexpr auto end = 10.0;
/*    auto rules = Rules{config.get_config(":Rules")};
    auto ssa = SSA{start, end, network, rules};

    [&]()
    {
        do
        {
            network.print();
        }
        while (ssa.execute());
    }();
*/
    return 0;
}

