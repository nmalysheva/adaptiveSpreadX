#ifndef TYPES_DISTRIBUTION_HPP_
#define TYPES_DISTRIBUTION_HPP_

#include "DistributionImpl.hpp"

#include <memory>
#include <vector>


/*!
 * \brief Customisable real distribution.
 *
 * Depending on the initilisation of an object, the distribution
 * will draw random numbers from a set distribution or return a fixed value.
 * The value drawn must be in the range [0, 1].
 */
class Distribution final
{
  public:
    /// type of the drawn numbers
    using value_type = double;

    /*!
     * \brief Create a random number distribution with given parameters.
     *
     *  Each call to `draw()` will create a random number from given `distribution` with parameters
     *  of vector  `params`.
     *  For example `Distribution{'U', {0.1, 0.4}}` will result in random numbers from a uniform
     *  distribution within the range `[0.1, 0.4]`.
     *
     *  These distributions are supported:
     *  - U(a,b) = Uniform distribution in the range [a, b]
     *  - N(a,b) = Normal distribution with mean = a and standard deviation = b
     *  - E(a) = Exponential distribution with rate = a
     *
     * \throws `std::invalid_argument` `distribution` is not supported
     * \throws `std::invalid_argument` wrong number of parameters given
     * \throws whatever the ctor of the distributions throw if the given parameters are incorrect
     *
     * \param p parameters of the distribution
     */
    Distribution(char distribution, std::vector<value_type> const& params);

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

  private:
    /// the distribution to draw from
    std::shared_ptr<DistributionImpl> m_dist{nullptr};
};

#endif

