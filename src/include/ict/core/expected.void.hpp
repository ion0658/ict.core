#pragma once

#ifndef ICT_CORE_EXPECTED_VOID_HPP
#define ICT_CORE_EXPECTED_VOID_HPP

#include <version>

#ifndef __cpp_lib_expected
#include <cassert>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

#include "ict/core/expected.hpp"

namespace ict::core {

// specialization for void
template <is_void T, expected_error_type E>
class [[nodiscard]] expected<T, E> {  // NOLINT
   private:
    using VariantType = std::variant<std::monostate, unexpected<E> >;
    VariantType _storage;

   public:
    using value_type = void;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    template <class U>
    using rebind = expected<U, error_type>;

    constexpr expected() : _storage(std::monostate{}) {}
    constexpr expected(const expected& rhs)
        requires(std::copy_constructible<E>)
    {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(_storage), std::monostate{});
        } else {
            std::construct_at(std::addressof(_storage), unexpected<E>(rhs.error()));
        }
    }

    constexpr expected(expected&& rhs) noexcept(std::is_nothrow_move_constructible_v<E>)
        requires(std::move_constructible<E>)
    {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(_storage), std::monostate{});
        } else {
            std::construct_at(std::addressof(_storage), unexpected<E>(std::move(rhs.error())));
        }
    };

    template <class U, class G>
        requires(std::is_void_v<U> && std::constructible_from<E, const G&> &&
                 !std::constructible_from<unexpected<E>, expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, expected<U, G>&> &&
                 !std::constructible_from<unexpected<E>, const expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, const expected<U, G>&>)
    constexpr explicit(!std::convertible_to<const G&, E>) expected(const expected<U, G>& rhs) {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(_storage), std::monostate{});
        } else {
            std::construct_at(std::addressof(_storage), unexpected<E>(std::forward<const G&>(rhs.error())));
        }
    }
    template <class U, class G>
        requires(std::is_void_v<U> && std::constructible_from<E, const G&> &&
                 !std::constructible_from<unexpected<E>, expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, expected<U, G>&> &&
                 !std::constructible_from<unexpected<E>, const expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, const expected<U, G>&>)
    constexpr explicit(!std::convertible_to<G, E>) expected(expected<U, G>&& rhs) {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(_storage), std::monostate{});
        } else {
            std::construct_at(std::addressof(_storage), unexpected<E>(std::forward<G>(rhs.error())));
        }
    }

    template <class G = E>
        requires(std::constructible_from<E, const G&>)
    constexpr explicit(!std::convertible_to<const G&, E>) expected(const unexpected<G>& err)
        : _storage(unexpected<E>(std::forward<const G&>(err.error()))) {}
    template <class G = E>
        requires(std::constructible_from<E, G>)
    constexpr explicit(!std::convertible_to<G, E>) expected(unexpected<G>&& err)
        : _storage(unexpected<E>(std::forward<G>(err.error()))) {}

    constexpr explicit expected(std::in_place_t) noexcept : expected() {}

    template <class... Args>
        requires(std::constructible_from<E, Args...>)
    constexpr explicit expected(unexpect_t, Args&&... args) : _storage(unexpected(std::forward<Args...>(args...))) {}

    template <class U, class... Args>
        requires(std::constructible_from<E, std::initializer_list<U>&, Args...>)
    constexpr explicit expected(unexpect_t, std::initializer_list<U> ilist, Args&&... args)
        : _storage(unexpected(ilist, std::forward<Args...>(args)...)) {}

    constexpr ~expected() = default;

    constexpr expected& operator=(const expected& rhs)
        requires(std::is_copy_assignable_v<E> && std::copy_constructible<E>)
    {
        if (this->has_value() && !rhs.has_value()) {
            std::construct_at(std::addressof(_storage), rhs._storage);
        }
        if (!this->has_value() && rhs.has_value()) {
            _storage = std::monostate{};
        }
        if (!this->has_value() && !rhs.has_value()) {
            _storage = rhs._storage;
        }
        return *this;
    }

    constexpr expected& operator=(expected&& rhs) noexcept(std::is_nothrow_move_constructible_v<E> &
                                                           std::is_nothrow_move_assignable_v<E>) {
        if (this->has_value() && !rhs.has_value()) {
            std::construct_at(std::addressof(_storage), std::move(rhs._storage));
        }
        if (!this->has_value() && rhs.has_value()) {
            _storage = std::monostate{};
        }
        if (!this->has_value() && !rhs.has_value()) {
            _storage = std::move(rhs._storage);
        }
        return *this;
    }

    template <class G = E>
        requires(std::constructible_from<E, const G&> && std::is_assignable_v<E&, const G&>)
    constexpr expected& operator=(const unexpected<G>& err) {
        if (this->has_value()) {
            std::construct_at(std::addressof(_storage), unexpected<E>(std::forward<const G&>(err.error())));
        } else {
            _storage = unexpected<E>(std::forward<const G&>(err.error()));
        }
        return *this;
    }

    template <class G = E>
        requires(std::constructible_from<E, G> && std::is_assignable_v<E&, G>)
    constexpr expected& operator=(unexpected<G>&& err) {
        if (this->has_value()) {
            std::construct_at(std::addressof(_storage), unexpected<E>(std::forward<G>(err.error())));
        } else {
            _storage = unexpected<E>(std::forward<G>(err.error()));
        }
        return *this;
    }

    constexpr void emplace() noexcept {
        if (!this->has_value()) {
            this->_storage = std::monostate{};
        }
    }

    constexpr void swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<E> &&
                                                std::is_nothrow_swappable_v<E>)
        requires(std::swappable<E> && std::move_constructible<E>)
    {
        _storage.swap(rhs._storage);
    }

    constexpr void operator*() const noexcept {}
    constexpr explicit operator bool() const noexcept { return has_value(); }
    constexpr bool has_value() const noexcept { return std::holds_alternative<std::monostate>(this->_storage); }

    constexpr void value() const& {
        if (!this->has_value()) {
            throw bad_expected_access<E>(this->error());
        }
    }
    constexpr void value() && {
        if (!this->has_value()) {
            throw bad_expected_access<E>(std::move(this->error()));
        }
    }

    constexpr const E& error() const& noexcept {
        assert(!this->has_value());
        return std::get<unexpected<E> >(this->_storage).error();
    }
    constexpr E& error() & noexcept {
        assert(!this->has_value());
        return std::get<unexpected<E> >(this->_storage).error();
    }
    constexpr const E&& error() const&& noexcept {
        assert(!this->has_value());
        return std::move(std::get<unexpected<E> >(this->_storage).error());
    }
    constexpr E&& error() && noexcept {
        assert(!this->has_value());
        return std::move(std::get<unexpected<E> >(this->_storage).error());
    }

    template <class G = E>
        requires(std::copy_constructible<E> && std::convertible_to<G, E>)
    constexpr E error_or(G&& err) const& {
        return this->has_value() ? std::forward<G>(err) : this->error();
    }

    template <class G = E>
        requires(std::move_constructible<E> && std::convertible_to<G, E>)
    constexpr E error_or(G&& err) && {
        return this->has_value() ? std::forward<G>(err) : std::move(this->error());
    }

    template <class F>
        requires(std::copy_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F>::value_type, E> and_then_impl(F&& func) & {
        using R = std::remove_cvref_t<std::invoke_result_t<F> >;
        using V = R::value_type;
        using Err = R::error_type;
        if (this->has_value()) {
            return std::invoke(std::forward<F>(func));
        } else {
            return expected<V, Err>(unexpect, error());
        }
    }
    template <class F>
        requires(std::move_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> and_then_impl(F&& func) && {
        using R = std::remove_cvref_t<std::invoke_result_t<F, T> >;
        using V = R::value_type;
        using Err = R::error_type;
        if (this->has_value()) {
            return std::invoke(std::forward<F>(func));
        } else {
            return expected<V, Err>(unexpect, std::move(error()));
        }
    }

    template <class F>
        requires(std::copy_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> or_else_impl(F&& func) & {
        using R = std::remove_cvref_t<std::invoke_result_t<F, E> >;
        using V = R::value_type;
        using Err = R::error_type;
        if (has_value()) {
            return expected<V, Err>();
        } else {
            return unexpected<Err>(std::invoke(std::forward<F>(func), error()));
        }
    }
    template <class F>
        requires(std::move_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> or_else_impl(F&& func) && {
        using R = std::remove_cvref_t<std::invoke_result_t<F, E> >;
        using V = R::value_type;
        using Err = R::error_type;
        if (has_value()) {
            return expected<V, Err>();
        } else {
            return unexpected<Err>(std::invoke(std::forward<F>(func), std::move(error())));
        }
    }

    template <class F>
        requires(std::copy_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> and_then(F&& func) & {
        return and_then_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::copy_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> and_then(F&& func) const& {
        return and_then_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> and_then(F&& func) && {
        return and_then_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> and_then(F&& func) const&& {
        return and_then_impl(std::forward<F>(func));
    }

    template <class F>
        requires(std::copy_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> or_else(F&& func) & {
        return or_else_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::copy_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> or_else(F&& func) const& {
        return or_else_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> or_else(F&& func) && {
        return or_else_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> or_else(F&& func) const&& {
        return or_else_impl(std::forward<F>(func));
    }

    template <class F>
        requires(std::copy_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> transform(F&& func) & {
        return and_then_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::copy_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> transform(F&& func) const& {
        return and_then_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> transform(F&& func) && {
        return and_then_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<E> &&
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> transform(F&& func) const&& {
        return and_then_impl(std::forward<F>(func));
    }

    template <class F>
        requires(std::copy_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> transform_error(F&& func) & {
        return or_else_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::copy_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> transform_error(F&& func) const& {
        return or_else_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> transform_error(F&& func) && {
        return or_else_impl(std::forward<F>(func));
    }
    template <class F>
        requires(std::move_constructible<T> &&
                 std::same_as<T, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::value_type>)
    constexpr expected<T, typename std::invoke_result_t<F, E>::error_type> transform_error(F&& func) const&& {
        return or_else_impl(std::forward<F>(func));
    }
    template <class T2, class E2>
        requires requires(expected<T, E> lhs, expected<T2, E2> rhs) {
            std::is_void_v<T2>;
            lhs.error() == rhs.error();
            std::is_convertible_v<bool, decltype(lhs.error() == rhs.error())>;
        }
    friend constexpr bool operator==(const expected& lhs, const expected<T2, E2>& rhs) {
        if (lhs.has_value() != rhs.has_value()) {
            return false;
        }
        return lhs.has_value() || static_cast<bool>(lhs.error() == rhs.error());
    }
    template <class E2>
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    friend constexpr bool operator==(const expected& exp, const unexpected<E2>& err) {
        return !exp.has_value() && static_cast<bool>(exp.error() == err.error());
    }
    template <class E2>
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    friend constexpr bool operator==(const unexpected<E2>& err, const expected& exp) {
        return exp == err;
    }

    template <class T2, class E2>
        requires requires(expected<T, E> lhs, expected<T2, E2> rhs) {
            std::is_void_v<T2>;
            lhs.error() != rhs.error();
            std::is_convertible_v<bool, decltype(lhs.error() == rhs.error())>;
        }
    friend constexpr bool operator!=(const expected& lhs, const expected<T2, E2>& rhs) {
        return !(lhs == rhs);
    }
    template <class E2>
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    friend constexpr bool operator!=(const expected& exp, const unexpected<E2>& err) {
        return !(exp == err);
    }
    template <class E2>
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    friend constexpr bool operator!=(const unexpected<E2>& err, const expected& exp) {
        return exp != err;
    }
};

}  // namespace ict::core

template <ict::core::is_void T, typename E, typename CharT>
struct std::formatter<ict::core::expected<T, E>, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const ict::core::expected<T, E>& exp, auto& ctx) const {
        if (exp.has_value()) {
            return std::format_to(ctx.out(), "Ok(())");
        } else {
            return std::format_to(ctx.out(), "Err({})", exp.error());
        }
    }
};

#endif

#endif  // ICT_CORE_EXPECTED_VOID_HPP

