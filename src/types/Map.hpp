#ifndef TYPES_MAP_HPP_
#define TYPES_MAP_HPP_

#include <cassert>
#include <vector>
#include <utility>


template <typename K,
         typename V>
class Map final
{
  public:
    auto put(K&& k, V&& v) -> bool
    {
        if (find(k))
        {
            return false;
        }
        
        auto const pos = std::upper_bound(m_keys.begin(), m_keys.end(), k);
        auto const dist = std::distance(m_keys.begin(), pos);
        m_keys.emplace(pos, std::move(k));
        auto const pos_v = std::next(m_values.begin(), dist);
        m_values.emplace(pos_v, std::move(v));

        assert(std::is_sorted(m_keys.begin(), m_keys.end()));
        return true;
    }

    auto get(K const& k) const noexcept -> V const&
    {
        if (not find(k))
        {
            return bu;
        }
        auto const pos = std::lower_bound(m_keys.begin(), m_keys.end(), k);
        auto const index = std::distance(m_keys.begin(), pos);
        return m_values[index];
    }

  private:
    std::vector<K> m_keys;
    std::vector<V> m_values;
    V const bu{Propability{0}};

    auto find(K const& k) const noexcept -> bool
    {
        return std::binary_search(m_keys.begin(), m_keys.end(), k);
    }
};

#endif

