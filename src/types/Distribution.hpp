#ifndef TYPES_DISTRIBUTION_HPP_
#define TYPES_DISTRIBUTION_HPP_

#include <functional>

/*!
 * \brief Customisable real distribution.
 *
 * Depending on the initilisation of an object, the distribution
 * will draw random numbers from a set distribution or
 * return a fixed value.
 * The value drawn must be in the range [0, 1].
 */
class Distribution final
{
  public:
    /// Distribution will not yield a value in the range [0, 1].
    static constexpr auto OutOfRange = "distribution will not yield values from range [0, 1]";

    /// type of the drawn numbers
    using value_type = double;

    /*!
     * \brief Create a random number distribution with given parameters.
     *
     *  Each call to `draw()` will create a random number from given `distribution` with parameters
     *  `a` and `b`.
     *  For example `Distribution{'U', 0.1, 0.4}` will result in random numbers from a uniform
     *  distribution within the range `[0.1, 0.4]`.
     *
     * \throws `std::invalid_argument` `distribution` is not supported
     * \throws `std::invalid_argument` condition of `a` and `b` for given `distribution` violated
     *
     * \note Currently only U for uniform distribution is supported.
     *
     * \param distribution identifier of the distribution
     * \param a first parameter of the distribution (e.g. minimum value for uniform)
     * \param b second parameter of the distribution (e.g. maximum value for uniform)
     */
    Distribution(char distribution, value_type a, value_type b);

    /*!
     * \brief Create a fixed value distribution.
     *
     * Each call of `draw()` will return the specified value.
     *
     * \param value value that will always be returned
     */
    explicit Distribution(value_type value);

    /*!
     * \brief Draw number from the distribution.
     *
     * \return depending on the used constructor a random number of a fixed value
     */
    [[nodiscard]]
    auto draw() const -> value_type;

    /*!
     * \brief Draw n times and accumulate the values.
     *
     * \note This function draws n times and does not make use of any optimisations.
     * (e.g. Irwin-Hall distribution)
     *
     * \param n number of draws
     *
     * \return sum of `n` drawn numbers
     */
    [[nodiscard]]
    auto draw(unsigned n) const -> value_type;

    /// get minimum possible value of the distribution
    [[nodiscard]]
    auto min() const noexcept -> value_type;

    /// get maximum possible value of the distribution
    [[nodiscard]]
    auto max() const noexcept -> value_type;

  private:
    /// random number draw function
    std::function<value_type(void)> m_draw{};
};

#endif

