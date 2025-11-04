#pragma once

#ifndef ICT_CORE_UNEXPECTED_HPP
#define ICT_CORE_UNEXPECTED_HPP

#include <version>

#ifdef __cpp_lib_expected
#include <expected>
#else
#include <concepts>
#include <format>
#include <type_traits>
#include <utility>
#endif

#ifdef __cpp_lib_expected

export namespace ict::core {
template <typename E>
using unexpected = std::unexpected<E>;
}  // namespace ict::core

#else

namespace ict::core {

template <class E>
class unexpected;  // NOLINT

template <typename T>
struct is_unexpected : std::false_type {};  // NOLINT
template <typename T>
struct is_unexpected<unexpected<T> > : std::true_type {};
template <typename T>
constexpr bool is_unexpected_v = is_unexpected<std::remove_cvref_t<T> >::value;  // NOLINT

template <class E>
concept unexpected_error_type =
    std::is_object_v<E> && !std::is_array_v<E> && !std::is_const_v<E> && !is_unexpected_v<E>;

template <unexpected_error_type E>
class [[nodiscard]] unexpected<E> {  // NOLINT
   private:
    E _err;

   public:
    constexpr unexpected(const unexpected&) = default;
    constexpr unexpected(unexpected&&)
        requires(std::movable<E>)
    = default;

    template <class Err = E>
        requires(!std::same_as<std::remove_cvref_t<Err>, unexpected> &&
                 !std::same_as<std::remove_cvref_t<Err>, std::in_place_t> && std::constructible_from<E, Err>)
    constexpr explicit unexpected(Err&& val) : _err(static_cast<E>(std::forward<Err>(val))) {}

    template <class... Args>
        requires(std::constructible_from<E, Args...>)
    constexpr explicit unexpected(std::in_place_t, Args&&... args) : _err(std::forward<Args...>(args...)) {}

    template <class U, class... Args>
        requires(std::constructible_from<E, std::initializer_list<U>&, Args...>)
    constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> i_l, Args&&... args)
        : _err(i_l, std::forward<Args...>(args...)) {}

    constexpr ~unexpected()
        requires(std::is_trivially_destructible_v<E>)
    {
        _err.~E();
    }

    constexpr unexpected& operator=(const unexpected&)
        requires(std::copyable<E>)
    = default;
    constexpr unexpected& operator=(unexpected&&)
        requires(std::movable<E>)
    = default;

    constexpr const E& error() const& noexcept
        requires(std::movable<E>)
    {
        return _err;
    }
    constexpr E& error() & noexcept
        requires(std::movable<E>)
    {
        return _err;
    }
    constexpr const E&& error() const&& noexcept
        requires(std::movable<E>)
    {
        return std::move(_err);
    }
    constexpr E&& error() && noexcept
        requires(std::movable<E>)
    {
        return std::move(_err);
    }

    constexpr void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>)
        requires(std::swappable<E>)
    {
        std::swap(_err, other._err);
    }

    template <class E2>
        requires(std::equality_comparable_with<E, E2>)
    friend constexpr bool operator==(const unexpected& lhs, const unexpected<E2>& rhs) {
        return lhs.error() == rhs.error();
    }
    template <class E2>
        requires(std::equality_comparable_with<E, E2>)
    friend constexpr bool operator!=(const unexpected& lhs, const unexpected<E2>& rhs) {
        return lhs.error() != rhs.error();
    }
};

template <class E>
unexpected<typename std::decay<E>::type> make_unexpected(E&& err) {
    return unexpected<typename std::decay<E>::type>(std::forward<E>(err));
}

struct unexpect_t {  // NOLINT
    unexpect_t() = default;
};
inline constexpr unexpect_t unexpect{};  // NOLINT

template <class E>
unexpected(E) -> unexpected<E>;

template <class E>
constexpr void swap(unexpected<E>& lhs, unexpected<E>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

}  // namespace ict::core

template <typename T>
struct std::formatter<ict::core::unexpected<T>, char> : std::formatter<std::string, char> {
    std::format_context::iterator format(const ict::core::unexpected<T>& err, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", err.error());
    }
};
template <typename T>
struct std::formatter<ict::core::unexpected<T>, wchar_t> : std::formatter<std::wstring, wchar_t> {
    std::wformat_context::iterator format(const ict::core::unexpected<T>& err, std::wformat_context& ctx) const {
        return std::format_to(ctx.out(), "{}", err.error());
    }
};

#endif

#endif  // ICT_CORE_UNEXPECTED_HPP

