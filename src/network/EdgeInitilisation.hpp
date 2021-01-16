#ifndef NETWORK_EDGEINITILISATION_HPP_
#define NETWORK_EDGEINITILISATION_HPP_

#include <utils/Parse.hpp>

#include <cstddef>
#include <optional>
#include <tuple>


struct EdgeInitilisation final
{
    using value_type = EdgeInitilisation;
    using ParserTypes = std::tuple<std::size_t>;
    static constexpr auto Header = "Edges";

    std::optional<std::size_t> m_count{};

    EdgeInitilisation() = default;
    EdgeInitilisation(std::size_t const) noexcept;

    [[nodiscard]]
    auto exists(EdgeInitilisation const&) const noexcept -> bool;

    auto put(std::string const& val) noexcept -> bool;

    auto count() const noexcept -> std::size_t;
};

using EdgeInitilisations = EdgeInitilisation;

#endif

