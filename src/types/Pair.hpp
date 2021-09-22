#ifndef TYPES_PAIR_HPP_
#define TYPES_PAIR_HPP_

#include <utility>


/*!
 * \brief Representation of "something" that depends on "something else".
 *
 * E.g. it can describes the change of a state with a given rate.
 * 
 * \tparam D the type of the dependence
 * \tparam T the type that identifies the object that will change
 */
template <typename D, typename T>
struct Pair final
{
    /*!
     * \brief Initialise the object.
     *
     * \param d the value of the rate
     * \param id the identifier of the affected "something"
     */
    Pair(D d, T id)
        : rate{std::move(d)}, identifier{std::move(id)}
    {
    }

    /// the dependence rate
    D rate;
    
    /// identifier of "something"
    T identifier;
};

/// Compare two Pairs (with same templates) by compareing their identifier::operator<
template <typename D, typename T>
[[nodiscard]]
auto operator<(Pair<D, T> const& lhs, Pair<D, T> const& rhs) noexcept -> bool
{
    return lhs.identifier < rhs.identifier;
}

#endif

