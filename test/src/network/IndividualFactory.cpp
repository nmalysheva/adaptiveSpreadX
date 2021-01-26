#include <catch.hpp>

#include <network/IndividualFactory.hpp>

using namespace network;


TEST_CASE("individual_factory_make")
{
    auto const s = State{"A"};
    auto const n = 0.1;
    auto const r = 0.3;

    auto f = IndividualFactory{};
    f.add(s, Distribution{n}, Distribution{r});
    auto const i = f.make(s);

    REQUIRE(i.state() == s);
    REQUIRE(i.new_contact_rate() == n);
    REQUIRE(i.remove_contact_rate() == r);
}

TEST_CASE("individual_factory_error")
{
    auto const s = State{"A"};
    auto f = IndividualFactory{};
    f.add(s, Distribution{0}, Distribution{0});
    try
    {
        f.add(s, Distribution{1}, Distribution{1});
        FAIL();
    }
    catch (std::logic_error const& e)
    {
        REQUIRE(std::string(e.what()) == IndividualFactory::DuplicateState);
    }
    catch (...)
    {
        FAIL();
    }
}

