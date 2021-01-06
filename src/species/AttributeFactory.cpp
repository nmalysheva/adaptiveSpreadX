#include <species/AttributeFactory.hpp>
#include <configuration/Parser.hpp>


AttributeFactory::AttributeFactory(std::string const& n, std::string const& l)
{
    auto [ncr] = parse<Propability>(n);
    draw_new_contact_rate = [ncr] () { return ncr; };

    auto [lcr] = parse<Propability>(l);
    draw_loose_contact_rate = [lcr] () { return lcr; };
}
    

auto AttributeFactory::create() const -> std::pair<Propability, Propability>
{
    return {draw_new_contact_rate(), draw_loose_contact_rate() };
}

