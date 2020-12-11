#include <species/AttributeFactory.hpp>
#include <utils/parser.hpp>


AttributeFactory::AttributeFactory(std::string const& n, std::string const& l)
{
    auto [ncr] = parse<double>(n);
    draw_new_contact_rate = [ncr] () { return ncr; };

    auto [lcr] = parse<double>(l);
    draw_loose_contact_rate = [lcr] () { return lcr; };
}
    

auto AttributeFactory::create() const -> std::pair<double, double>
{
    return {draw_new_contact_rate(), draw_loose_contact_rate() };
}

