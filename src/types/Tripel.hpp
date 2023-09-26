#ifndef TYPES_TRIPEL_HPP_
#define TYPES_TRIPEL_HPP_

#include <utility>


/*!
 * \brief Combines two "somethings" that are connected, but depend on a rate.
 *
 * E.g. an int is converted and appended to a string with a given rate.
 * 
 * \tparam D the type of the dependence
 * \tparam T the type of the first "thing"
 * \tparam U the type of the second "thing"
 */
template <typename D,
          typename T,
          typename U>
struct Tripel final
{

    /*!
     * \brief Initialise the object.
     *
     * \param d the value of the rate
     * \param src the identifier of the first "something"
     * \param dest the identifier of the second "something"
     */
    Tripel(D d, T src, U dest)
        : rate{std::move(d)}, from{std::move(src)}, to{std::move(dest)}
    {
    }

//    auto operator=(Tripel const& rhs) -> Tripel& = default;

    /// rate for the event
    D rate;

    /// from "something"
    T from;

    /// to "something"
    U to;
};


/*!
 * \brief Compare two Tripel (with same templates) by their "somethings".
 *
 * It is compared by:
 * - `lhs.from < rhs.from == true` this comparison is `true`
 * - if `lhs.from == rhs.from` the result depends on the order of the `to`s
 * - otherwise it is `false`
 *
 * \tparam D \see Tripel
 * \tparam T \see Tripel
 * \tparam U \see Tripel
 *
 * \param lhs left hand side of the comparison
 * \param rhs right hand side of the comparison
 *
 * \return result, see description of this function
 */ 
template <typename D, typename T, typename U>
[[nodiscard]]
auto operator<(Tripel<D,T,U> const& lhs, Tripel<D, T, U> const& rhs) noexcept -> bool
{
    if (lhs.from == rhs.from)
    {
        return lhs.to < rhs.to;
    }

    return lhs.from < rhs.from;
}

#endif

