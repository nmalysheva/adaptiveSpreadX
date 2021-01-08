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
 * This class guarantees that a propability is in the range [0, 1.0].
 * Also it allows to express the intention in the code compared to the direct use
 * of e.g. double.
 */
class Propability final
{
  public:
    /// Encapsulated type.
    using value_type = double;

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
    explicit Propability(value_type const value);

    /// Get the encapsulated value.
    [[nodiscard]]
    explicit operator value_type() const noexcept;

  private:
    /// The value.
    value_type m_value{0.0};
};


/// lhs < rhs if the underlaying values compare <.
[[nodiscard]]
auto operator<(Propability const lhs, Propability const rhs) noexcept -> bool;

#endif

