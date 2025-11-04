#pragma once

#ifndef ICT_CORE_TRAIT_NOT_COPYABLE_HPP
#define ICT_CORE_TRAIT_NOT_COPYABLE_HPP

namespace ict::core {

struct NotCopyable {
    NotCopyable() = default;
    NotCopyable(const NotCopyable&) = delete;
    NotCopyable& operator=(const NotCopyable&) = delete;
    NotCopyable(NotCopyable&&) = default;
    NotCopyable& operator=(NotCopyable&&) = default;
};

struct INotCopyable {
    constexpr INotCopyable() = default;

   private:
    [[maybe_unused]] NotCopyable _not_copyable_t;
};

}  // namespace ict::core

#endif  // ICT_CORE_TRAIT_NOT_COPYABLE_HPP

