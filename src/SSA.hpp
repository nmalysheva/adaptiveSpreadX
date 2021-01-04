#ifndef SSA_HPP_
#define SSA_HPP_

#include <ContactNetwork.hpp>
#include <rules/Rules.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>


class SSA final
{
  public:
    SSA() = delete;
    SSA(double const start, double const end, ContactNetwork& network, Rules& rules) noexcept;

    auto execute() -> bool;

  private:
    double m_now;
    double const m_end;
    ContactNetwork& m_network;
    Rules& m_rules;
   
    //enum? 
    static constexpr auto Edge_Delete = -4;
    static constexpr auto Edge_Create = -3;
    static constexpr auto Birth = -2;
    static constexpr auto Death = -1;


    template <typename T>
    static auto find_action(T const& container, double const value) -> typename T::const_iterator
    {
        auto const result = std::find_if(std::cbegin(container), std::end(container), [value](auto const it) { return it.first > value; });
        assert(result not_eq std::cend(container));
        return result;
    }
};

#endif

