#include <configuration/ConfigurationFile.hpp>
#include <ContactNetwork.hpp>
#include <species/Species.hpp>
#include <SSA.hpp>

#include <iostream>


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return -1;
    }

    auto config = ConfigurationFile{argv[1]};
    auto const species = Species{config.get_config(":Species")};
    auto network = ContactNetwork{config.get_config(":Graph"), species};
    constexpr auto start = 0.0;
    constexpr auto end = 10.0;
    auto rules = Rules{config.get_config(":Rules")};
    auto ssa = SSA{start, end, network, rules};

    [&]()
    {
        do
        {
            network.print();
        }
        while (ssa.execute());
    }();

    return 0;
}

