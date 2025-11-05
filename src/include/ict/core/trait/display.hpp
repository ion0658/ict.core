#pragma once

#ifndef ICT_CORE_TRAIT_DISPLAY_HPP
#define ICT_CORE_TRAIT_DISPLAY_HPP

#include <format>
#include <string>

#include "ict/core/concepts.hpp"

namespace ict::core {
///
/// @brief IDisplay - Display interface
/// @details
/// struct IDisplay - human-readable string representation interface
///
struct IDisplay {
    virtual ~IDisplay() = default;
    ///
    /// @brief displays the object as a string
    /// @details this function should return a human-readable string representation of the object
    /// it will be used int format
    ///
    /// @return string representation of the object
    ///
    virtual std::string display() const = 0;
};

///
/// @brief concept is_displayable - checks if a type derives from IDisplay
///
template <typename T>
concept is_displayable = ict::core::derived_from_and_not_same<IDisplay, T>;

}  // namespace ict::core

///
/// @brief std::formatter specialization for ict::core::IDisplay derived types
///
template <ict::core::is_displayable T, typename CharT>
struct std::formatter<T, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const T& val, auto& ctx) const { return std::format_to(ctx.out(), "{}", val.display()); }
};

#endif  // ICT_CORE_TRAIT_DISPLAY_HPP

