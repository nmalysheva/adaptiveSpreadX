#ifndef TYPES_PROPABILITY_HPP_
#define TYPES_PROPABILITY_HPP_

/*! \file
 * \author Mathias Lindemann
 *
 * Contains propability functionality.
 */

/*!
 * \brief Representation of propability.
 *
 * This class guarantees that a propability is in the proper range (0 - 100).
 * Also it allows to express the intention in the code compared to the direct use
 * of e.g. double.
 */
class Propability final
{
  public:
    /*!
     * \brief Validates a given value.
     *
     * A value is valid if its between 0 and 1 (including).
     * If the value is valid, it is returned. 
     * Otherwise a std::out_of_range exception is thrown.
     *
     * \throws std::out_of_range valus is no in range [0, 1].
     *
     * \param value The value to check.
     * \returns The given value.
     */
    static
    auto validate(double const value) -> double;

    /// Create 0 %.
    Propability() noexcept = default;

    /*!
     * \brief Initialise with a given value.
     *
     * The value must be between 0 and 1 (including).
     *
     * \throws std::out_of_range value violates requirements.
     * 
     * \param value The initilisation value.
     */
    explicit Propability(double const value);

    /// Get the encapsulated value.
    explicit operator double() const noexcept;

  private:
    /// The value.
    double m_value{0.0};
};

#endif

