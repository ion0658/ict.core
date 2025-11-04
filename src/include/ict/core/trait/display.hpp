#pragma once

#ifndef ICT_CORE_TRAIT_DISPLAY_HPP
#define ICT_CORE_TRAIT_DISPLAY_HPP

#include <format>
#include <string>

#include "ict/core/concepts.hpp"

namespace ict::core {
struct IDisplay {
    virtual ~IDisplay() = default;
    virtual std::string display() const = 0;
};
}  // namespace ict::core

template <typename T, typename CharT>
    requires ict::core::derived_from_and_not_same<ict::core::IDisplay, T>
struct std::formatter<T, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const T& val, auto& ctx) const { return std::format_to(ctx.out(), "{}", val.display()); }
};

#endif  // ICT_CORE_TRAIT_DISPLAY_HPP

