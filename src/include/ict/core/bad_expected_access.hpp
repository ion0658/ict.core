#pragma once

#ifndef ICT_CORE_EXCEPTION_HPP
#define ICT_CORE_EXCEPTION_HPP

#include <version>

#ifdef __cpp_lib_expected

#include <expected>

// use std::bad_expected_access if available
namespace ict::core {
template <typename E>
using bad_expected_access = std::bad_expected_access<E>;
}  // namespace ict::core

#else
#include <concepts>
#include <exception>

namespace ict::core {

///
/// @brief base class for exceptions thrown by `expected` when accessing the value
///
/// @tparam E Error type
///
template <typename E>
class bad_expected_access : public std::exception {  // NOLINT
   private:
    E _error;

   public:
    constexpr explicit bad_expected_access(E err) : _error(std::move(err)) {}

    constexpr bad_expected_access(const bad_expected_access&)
        requires(std::copy_constructible<E>)
    = default;
    constexpr bad_expected_access(bad_expected_access&&)
        requires(std::move_constructible<E>)
    = default;

    virtual ~bad_expected_access() = default;

    bad_expected_access& operator=(const bad_expected_access&)
        requires(std::copyable<E>)
    = default;

    constexpr const E& error() const& noexcept { return _error; }

    constexpr E& error() & noexcept { return _error; }

    constexpr const E&& error() const&& noexcept { return std::move(_error); }

    constexpr E&& error() && noexcept { return std::move(_error); }

    constexpr const char* what() const noexcept override {
        return "bad access to std::expected without expected value";
    }
};

}  // namespace ict::core

#endif

#endif  // ICT_CORE_EXCEPTION_HPP

