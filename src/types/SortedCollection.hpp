#ifndef TYPES_SORTEDCOLLECTION_HPP_
#define TYPES_SORTEDCOLLECTION_HPP_

#include <algorithm>
#include <cassert>
#include <string>
#include <utility>
#include <vector>


template <typename T>
class SortedCollection final
{
  public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = T const&;

    SortedCollection() = default;

    /*template <typename U>
    auto operator[](U const& val) -> reference_type
    {
        assert(exists(val));
        return *std::lower_bound(m_data.begin(), m_data.end(), val);
    }*/

    template <typename U>
    auto operator[](U const& val) const -> const_reference_type
    {
        assert(exists(val));
        return *std::lower_bound(m_data.begin(), m_data.end(), val);
    }

    template <typename U>
    auto exists(U const& val) const -> bool
    {
        auto const it = std::lower_bound(m_data.cbegin(), m_data.cend(), val);
        if (it == m_data.cend())
        {
            return false;
        }

        return *it == val;
    }

    auto put(std::string const& str) -> bool
    {
        /// return fasle or throw?
        auto val = value_type{str};

        if (exists(val))
        {
            return false;
        }

        m_data.emplace_back(std::move(val));
        std::sort(m_data.begin(), m_data.end());
        return true;
    }

    auto data() const noexcept -> std::vector<value_type> const&
    {
        return m_data;
    }

  private:
    std::vector<value_type> m_data{};
};

#endif

