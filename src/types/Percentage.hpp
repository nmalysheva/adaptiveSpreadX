#ifndef TYPES_PERCENTAGE_HPP_
#define TYPES_PERCENTAGE_HPP_

/*! \file
 * \author Mathias Lindemann
 *
 * Contains percentage functionality.
 */

/*!
 * \brief Representation of percentage.
 *
 * This class guarantees that a percentage is in the proper range (0 - 100).
 * Also it allows to express the intention in the code compared to the direct use
 * of e.g. double.
 */
class Percentage final
{
  public:
    /// Create 0 %.
    Percentage() noexcept = default;

    /*!
     * \brief Initialise with a given value.
     *
     * The value must be between 0 and 1 (including).
     *
     * \throws std::out_of_range value violates requirements.
     * 
     * \param value The initilisation value.
     */
    Percentage(double const value);

  private:
    /// The value.
    double m_value{0.0};

    /*!
     * \brief Check if the current value is between 0 and 1 (inclusive).
     *
     * If not, a std::out_of_range exception is thrown.
     *
     * \throws std::out_of_range 0 <= m_value <= 1 is not given.
     */
    auto check() const -> void;
};

#endif

