#ifndef SPECIES_PEOPLE_FACTORY_HPP_
#define SPECIES_PEOPLE_FACTORY_HPP_

#include <functional>
#include <string>
#include <utility>


/*!
 * \brief Generate values for attributes of an individual, according to the given parameters.
 *
 * The factory takes two strings and determines how to generate the needed attribute values for
 * a certain class of individuals.
 *
 * These attributes can be
 * - constant values -> each time the same values are returned
 * - parameters of a propability distribution -> on each creation the attributes are randomly drawn
 *
 * \todo Where to put discription of formats?
 */
class AttributeFactory final
{
  public:
    /*!
     * \brief Set up the factory.
     *
     * \param n New contact rate
     * \param l Loose contact rate
     */
    AttributeFactory(std::string const& n, std::string const& l);

    /// Return attribute values.
    [[nodiscard]]
    auto create() const -> std::pair<double, double>;


  private:
    std::function<double(void)> draw_new_contact_rate;
    std::function<double(void)> draw_loose_contact_rate;
};

#endif

