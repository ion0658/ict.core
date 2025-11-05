#pragma once

#ifndef ICT_CORE_TRAIT_NOT_COPYABLE_HPP
#define ICT_CORE_TRAIT_NOT_COPYABLE_HPP

#include <concepts>
#include <type_traits>

#include "ict/core/concepts.hpp"

namespace ict::core {

///
/// @brief NotCopyable - disallow copy operations
///
struct NotCopyable {
    NotCopyable() = default;
    NotCopyable(const NotCopyable&) = delete;
    NotCopyable& operator=(const NotCopyable&) = delete;
    NotCopyable(NotCopyable&&) = default;
    NotCopyable& operator=(NotCopyable&&) = default;
};

///
/// @brief INotCopyable - interface to disallow copy operations
///
struct INotCopyable {
    constexpr INotCopyable() = default;

   private:
    [[maybe_unused]] NotCopyable _not_copyable_t;
};

///
/// @brief concept not_copyable - checks if T is not copyable
///
template <typename T>
concept not_copyable =
    not(std::copyable<T> && std::is_copy_assignable_v<T>) || ict::core::derived_from_and_not_same<INotCopyable, T>;

}  // namespace ict::core

#endif  // ICT_CORE_TRAIT_NOT_COPYABLE_HPP

