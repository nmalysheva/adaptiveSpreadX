#ifndef TYPES_FACTORY_HPP_
#define TYPES_FACTORY_HPP_

#include <string>
#include <tuple>
#include <utility>


template <typename T>
class Factory final
{
  public:
    using value_type = T;
    constexpr static auto Header = T::Header;
    using ParserTypes = typename T::ParserTypes;

    Factory() = delete;

    Factory(std::string const& str)
        : m_data{parse::split<std::tuple_size_v<ParserTypes>>(str)}
    {
    }

    Factory(ParserTypes values)
    : m_data{values}
    {
    }

    [[nodiscard]]
    auto make() const -> T
    {
        return std::make_from_tuple<T>(make(std::make_index_sequence<std::tuple_size_v<ParserTypes>>{}));
    }

    template <typename... Us>
    [[nodiscard]]
    auto operator<(Us const&... rhs) const noexcept -> bool
    {
        static_assert(sizeof... (Us) == T::CompareIndices);
        return get_comparables() < std::make_tuple(rhs...);
    }

    [[nodiscard]]
    auto operator<(Factory<T> const& rhs) const noexcept -> bool
    {
        return get_comparables() < rhs.get_comparables();
    }

    template <typename... Us>
    [[nodiscard]]
    auto operator==(Us const&... rhs) const noexcept -> bool
    {
        static_assert(sizeof... (Us) == T::CompareIndices);
        return get_comparables() == std::make_tuple(rhs...);
    }

    [[nodiscard]]
    auto operator==(Factory<T> const& rhs) const noexcept -> bool
    {
        return get_comparables() == rhs.get_comparables();
    }

  private:
    ParserTypes m_data;

    template <std::size_t... Is>
    [[nodiscard]]
    auto get_comparables_impl(std::index_sequence<Is...>) const noexcept
    {
        return std::make_tuple(std::get<Is>(m_data)...);
    }

    [[nodiscard]]
    auto get_comparables() const noexcept
    {
        return get_comparables_impl(std::make_index_sequence<T::CompareIndices>{});
    }

    template <std::size_t... Is>
    [[nodiscard]]
    auto make(std::index_sequence<Is...>) const
    {
        return std::make_tuple((std::get<Is>(m_data).operator()())...);
    }
};

#endif

