//#include "dynamics.hpp"

#include <configuration/ConfigurationFile.hpp>
#include <ContactNetwork.hpp>
#include <species/Species.hpp>


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return -1;
    }

    auto config = ConfigurationFile{argv[1]};
    auto const species = Species{config.get_config(":Species")};
    auto network = ContactNetwork{config.get_config(":Graph"), species};

    return 0;
}

