#pragma once

#ifndef ICT_CORE_EXPECTED_HPP
#define ICT_CORE_EXPECTED_HPP

#include <format>
#include <version>

#ifdef __cpp_lib_expected

#include <expected>

namespace ict::core {
template <typename T, typename E>
using expected = std::expected<T, E>;
}  // namespace ict::core

#else

#include <cassert>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

#include "ict/core/bad_expected_access.hpp"
#include "ict/core/unexpected.hpp"

namespace ict::core {

template <class T, class E>
class [[nodiscard]] expected;

///
/// @brief Trait to check whether T is unexpected<U> for some U
///
template <class T>
concept is_qualified_expected =
    !std::is_reference_v<T> && !std::is_function_v<T> && !std::same_as<std::in_place_t, T> &&
    !std::same_as<unexpect_t, T> && !is_unexpected_v<T>;

///
/// @brief Trait to check whether T is a valid value type
///
template <class T>
concept expected_value_type = is_qualified_expected<T> && std::destructible<T>;
///
/// @brief Trait to check whether E is a valid error type
///
template <class E>
concept expected_error_type = is_qualified_expected<E> && unexpected_error_type<E> && std::destructible<E>;

///
/// @brief Concept to check whether T can be constructed or converted from W
///
template <class T, class W>
concept convert_from = std::constructible_from<T, W> || std::convertible_to<W, T>;

///
/// @brief Concept to check whether T can be constructed or converted from W with any cvref qualifier
///
template <class T, class W>
concept convert_from_any_cvref =
    convert_from<T, W&> || convert_from<T, W> || convert_from<T, const W&> || convert_from<T, const W>;

///
/// @brief Concept to check whether F can be invoked with Args and return R
///
template <class R, class F, class... Args>
concept invocable_with_args = std::is_invocable_r_v<R, F, Args...> || std::is_invocable_r_v<R, F, const Args...>;

template <typename T, typename E>
struct is_expected : std::false_type {};  // NOLINT
template <typename T, typename E>
struct is_expected<expected<T, E>, E> : std::true_type {};

///
/// @brief Variable template to check whether T is expected<U, E> for some U
///
template <typename T, typename E>
constexpr bool is_expected_v = is_expected<std::remove_cvref_t<T>, E>::value;  // NOLINT

///
/// @brief expected - either contains a value of type T or an error of type E
///
/// @tparam expected_value_type value type
/// @tparam expected_error_type error type
///
template <expected_value_type T, expected_error_type E>
class [[nodiscard]] expected<T, E> {  // NOLINT
   private:
    using VariantType = std::variant<T, unexpected<E> >;
    VariantType _storage;

   public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = unexpected<E>;
    template <class U>
    using rebind = expected<U, error_type>;

    constexpr expected()
        requires(std::is_default_constructible_v<T>)
        : _storage(T{}) {};
    constexpr expected(const expected& rhs)
        requires(std::copy_constructible<T> && std::copy_constructible<E>)
        : _storage(rhs._storage)

    {}
    constexpr expected(expected&& rhs) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                std::is_nothrow_move_constructible_v<E>)
        requires(std::move_constructible<T> && std::move_constructible<E>)
        : _storage(std::move(rhs._storage)) {}

    template <class U, class G>
        requires(std::constructible_from<T, const U&> && std::constructible_from<E, const G&> &&
                 !convert_from_any_cvref<T, expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, expected<U, G>&> &&
                 !std::constructible_from<unexpected<E>, expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, const expected<U, G>&> &&
                 !std::constructible_from<unexpected<E>, const expected<U, G> >)
    constexpr explicit(!std::convertible_to<const U&, T> || !std::convertible_to<const G&, E>)
        expected(const expected<U, G>& rhs) {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(this->_storage), T(std::forward<const U&>(rhs.value())));
        } else {
            std::construct_at(std::addressof(this->_storage), unexpected<E>(std::forward<const G&>(rhs.error())));
        }
    }
    template <class U, class G>
        requires(std::constructible_from<T, U> && std::constructible_from<E, G> &&
                 !convert_from_any_cvref<T, expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, expected<U, G>&> &&
                 !std::constructible_from<unexpected<E>, expected<U, G> > &&
                 !std::constructible_from<unexpected<E>, const expected<U, G>&> &&
                 !std::constructible_from<unexpected<E>, const expected<U, G> >)
    constexpr explicit(!std::convertible_to<U, T> || !std::convertible_to<G, E>) expected(expected<U, G>&& rhs) {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(this->_storage), T(std::forward<U>(rhs.value())));
        } else {
            std::construct_at(std::addressof(this->_storage), unexpected<E>(std::forward<G>(rhs.error())));
        }
    }

    template <class U = T>
        requires(!std::same_as<std::remove_cvref_t<U>, std::in_place_t> &&
                 !std::same_as<expected<T, E>, std::remove_cvref_t<U> > && !is_unexpected_v<U> &&
                 std::constructible_from<T, U>)
    constexpr explicit(!std::convertible_to<U, T>) expected(U&& val) : _storage(std::forward<U>(val)) {}

    template <class G = E>
        requires(std::constructible_from<E, const G&>)
    constexpr explicit(!std::convertible_to<const G&, E>) expected(const unexpected<G>& err) : _storage(err) {}
    template <class G = E>
        requires(std::constructible_from<E, G>)
    constexpr explicit(!std::convertible_to<G, E>) expected(unexpected<G>&& err) : _storage(err) {}

    template <class... Args>
        requires(std::constructible_from<T, Args...>)
    constexpr explicit expected(std::in_place_t, Args&&... args) {
        std::construct_at(std::addressof(this->_storage), T(std::forward<Args...>(args...)));
    }
    template <class U, class... Args>
        requires(std::constructible_from<T, std::initializer_list<U>&, Args...>)
    constexpr explicit expected(std::in_place_t, std::initializer_list<U> ilist, Args&&... args) {
        std::construct_at(std::addressof(this->_storage), T(ilist, std::forward<Args...>(args...)));
    }

    template <class... Args>
        requires(std::constructible_from<E, Args...>)
    constexpr explicit expected(unexpect_t, Args&&... args) {
        std::construct_at(std::addressof(this->_storage), unexpected<E>(std::forward<Args...>(args...)));
    }
    template <class U, class... Args>
        requires(std::constructible_from<E, std::initializer_list<U>&, Args...>)
    constexpr explicit expected(unexpect_t, std::initializer_list<U> ilist, Args&&... args) {
        std::construct_at(std::addressof(this->_storage), unexpected<E>(ilist, std::forward<Args...>(args...)));
    }

    constexpr ~expected() = default;

    constexpr expected& operator=(const expected& rhs)
        requires(std::is_copy_assignable_v<T> && std::copy_constructible<T> && std::is_copy_assignable_v<E> &&
                 std::copy_constructible<E> &&
                 (std::is_nothrow_move_constructible_v<T> || std::is_nothrow_move_constructible_v<E>))
    {
        _storage = rhs._storage;
        return *this;
    }

    constexpr expected& operator=(expected&& rhs) noexcept(std::is_nothrow_move_assignable_v<T> &&
                                                           std::is_nothrow_move_constructible_v<T> &&
                                                           std::is_nothrow_move_assignable_v<E> &&
                                                           std::is_nothrow_move_constructible_v<E>) {
        _storage = std::move(rhs._storage);
        return *this;
    }

    template <class U = T>
        requires(!std::same_as<expected, std::remove_cvref_t<U> > && !is_unexpected_v<std::remove_cvref_t<U> > &&
                 std::constructible_from<T, U> && std::assignable_from<T&, U> &&
                 (std::is_nothrow_constructible_v<T, U> || std::is_nothrow_move_constructible_v<T> ||
                  std::is_nothrow_move_constructible_v<E>))
    constexpr expected& operator=(U&& val) {
        _storage = std::forward<U>(val);
        return *this;
    }

    template <class G>
        requires(std::constructible_from<E, const G&> && std::assignable_from<E&, const G&> &&
                 (std::is_nothrow_constructible_v<E, const G&> || std::is_nothrow_move_constructible_v<T> ||
                  std::is_nothrow_move_constructible_v<E>))
    constexpr expected& operator=(const unexpected<G>& err) {
        _storage = unexpected<E>(std::forward<const G&>(err.error()));
        return *this;
    }

    template <class G>
        requires(std::constructible_from<E, G> && std::assignable_from<E&, G> &&
                 (std::is_nothrow_constructible_v<E, G> || std::is_nothrow_move_constructible_v<T> ||
                  std::is_nothrow_move_constructible_v<E>))
    constexpr expected& operator=(unexpected<G>&& err) {
        _storage = unexpected<E>(std::forward<G>(err.error()));
        return *this;
    }

    template <class... Args>
        requires(std::is_nothrow_constructible_v<T, Args...>)
    constexpr T& emplace(Args&&... args) noexcept {
        _storage = T(std::forward<Args>(args)...);
        return std::get<T>(this->_storage);
    }

    template <class U, class... Args>
        requires(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
    constexpr T& emplace(std::initializer_list<U> ilist, Args&&... args) noexcept {
        _storage = T(ilist, std::forward<Args>(args)...);
        return std::get<T>(this->_storage);
    }
    constexpr void swap(expected& rhs) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                std::is_nothrow_swappable_v<T> &&
                                                std::is_nothrow_move_constructible_v<E> &&
                                                std::is_nothrow_swappable_v<E>)
        requires(std::swappable<T> && std::swappable<E> && (std::move_constructible<T> && std::move_constructible<E>) &&
                 (std::is_nothrow_move_constructible_v<T> || std::is_nothrow_move_constructible_v<E>))
    {
        _storage.swap(rhs._storage);
    }

    constexpr const T* operator->() const noexcept {
        assert(this->has_value());
        return std::addressof(std::get<T>(this->_storage));
    }
    constexpr T* operator->() noexcept {
        assert(this->has_value());
        return std::addressof(std::get<T>(this->_storage));
    }
    constexpr const T& operator*() const& noexcept {
        assert(this->has_value());
        return std::get<T>(this->_storage);
    }
    constexpr T& operator*() & noexcept {
        assert(this->has_value());
        return std::get<T>(this->_storage);
    }
    constexpr T&& operator*() && noexcept {
        assert(this->has_value());
        return std::move(std::get<T>(this->_storage));
    }
    constexpr const T&& operator*() const&& noexcept {
        assert(this->has_value());
        return std::move(std::get<T>(this->_storage));
    }

    constexpr explicit operator bool() const noexcept { return has_value(); }
    constexpr bool has_value() const noexcept { return std::holds_alternative<T>(this->_storage); }

    constexpr const T& value() const& {
        if (!this->has_value()) {
            throw bad_expected_access<E>(this->error());
        }
        return std::get<T>(this->_storage);
    }
    constexpr T& value() & {
        if (!this->has_value()) {
            throw bad_expected_access<E>(this->error());
        }
        return std::get<T>(this->_storage);
    }
    constexpr const T&& value() const&& {
        if (!this->has_value()) {
            throw bad_expected_access<E>(std::move(this->error()));
        }
        return std::move(std::get<T>(this->_storage));
    }
    constexpr T&& value() && {
        if (!this->has_value()) {
            throw bad_expected_access<E>(std::move(this->error()));
        }
        return std::move(std::get<T>(this->_storage));
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

    template <class U>
        requires(std::copy_constructible<T> && std::convertible_to<U, T>)
    constexpr T value_or(U&& val) const& {
        return this->has_value() ? this->value() : static_cast<T>(std::forward<U>(val));
    }
    template <class U>
        requires(std::move_constructible<T> && std::convertible_to<U, T>)
    constexpr T value_or(U&& val) && {
        return this->has_value() ? std::move(this->value()) : static_cast<T>(std::forward<U>(val));
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
                 std::same_as<E, typename std::remove_cvref_t<std::invoke_result_t<F, T> >::error_type>)
    constexpr expected<typename std::invoke_result_t<F, T>::value_type, E> and_then_impl(F&& func) & {
        using R = std::remove_cvref_t<std::invoke_result_t<F, T> >;
        using V = R::value_type;
        using Err = R::error_type;
        if (this->has_value()) {
            return std::invoke(std::forward<F>(func), value());
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
            return std::invoke(std::forward<F>(func), std::move(value()));
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
            return expected<V, Err>(std::in_place, value());
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
            return expected<V, Err>(std::in_place, std::move(value()));
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
        requires(!std::is_void_v<T2>)
    friend constexpr bool operator==(const expected& lhs, const expected<T2, E2>& rhs)
        requires requires(expected<T, E> lhs, expected<T2, E2> rhs) {
            lhs.value() == rhs.value();
            std::is_convertible_v<bool, decltype(lhs.value() == rhs.value())>;
            lhs.error() == rhs.error();
            std::is_convertible_v<bool, decltype(lhs.error() == rhs.error())>;
        }
    {
        if (lhs.has_value() != rhs.has_value()) {
            return false;
        }
        if (lhs.has_value()) {
            return lhs.value() == rhs.value();
        } else {
            return lhs.error() == rhs.error();
        }
    }

    template <class T2>
        requires(!std::same_as<expected<T, E>, T2>)
    friend constexpr bool operator==(const expected& exp, const T2& val)
        requires requires(expected<T, E> exp, T2 val) {
            exp.value() == val;
            std::is_convertible_v<bool, decltype(exp.value() == val)>;
        }
    {
        return exp.has_value() && (exp.value() == val);
    }

    template <class T2>
        requires(!std::same_as<expected<T, E>, T2>)
    friend constexpr bool operator==(const T2& val, const expected& exp)
        requires requires(expected<T, E> exp, T2 val) {
            exp.value() == val;
            std::is_convertible_v<bool, decltype(exp.value() == val)>;
        }
    {
        return exp == val;
    }

    template <class E2>
    friend constexpr bool operator==(const expected& exp, const unexpected<E2>& err)
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    {
        return !exp.has_value() && (exp.error() == err.error());
    }

    template <class E2>
    friend constexpr bool operator==(const unexpected<E2>& err, const expected& exp)
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    {
        return exp == err;
    }

    template <class T2, class E2>
        requires(!std::is_void_v<T2>)
    friend constexpr bool operator!=(const expected& lhs, const expected<T2, E2>& rhs)
        requires requires(expected<T, E> lhs, expected<T2, E2> rhs) {
            lhs.value() == rhs.value();
            std::is_convertible_v<bool, decltype(lhs.value() == rhs.value())>;
            lhs.error() == rhs.error();
            std::is_convertible_v<bool, decltype(lhs.error() == rhs.error())>;
        }
    {
        return !(lhs == rhs);
    }

    template <class T2>
        requires(!std::same_as<expected<T, E>, T2>)
    friend constexpr bool operator!=(const expected& exp, const T2& val)
        requires requires(expected<T, E> exp, T2 val) {
            exp.value() == val;
            std::is_convertible_v<bool, decltype(exp.value() == val)>;
        }
    {
        return !(exp == val);
    }
    template <class T2>
        requires(!std::same_as<expected<T, E>, T2>)
    friend constexpr bool operator!=(const T2& val, const expected& exp)
        requires requires(expected<T, E> exp, T2 val) {
            exp.value() == val;
            std::is_convertible_v<bool, decltype(exp.value() == val)>;
        }
    {
        return !(exp == val);
    }

    template <class E2>
    friend constexpr bool operator!=(const expected& exp, const unexpected<E2>& err)
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    {
        return !(exp == err);
    }
    template <class E2>
    friend constexpr bool operator!=(const unexpected<E2>& err, const expected& exp)
        requires requires(expected<T, E> exp, unexpected<E2> err) {
            exp.error() == err.error();
            std::is_convertible_v<bool, decltype(exp.error() == err.error())>;
        }
    {
        return !(exp == err);
    }
};
template <class T, class E>
constexpr void swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}
template <class E>
constexpr void swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

}  // namespace ict::core

#endif

template <typename T, typename E, typename CharT>
struct std::formatter<ict::core::expected<T, E>, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const ict::core::expected<T, E>& exp, auto& ctx) const {
        if (exp.has_value()) {
            return std::format_to(ctx.out(), "Ok({})", exp.value());
        } else {
            return std::format_to(ctx.out(), "Err({})", exp.error());
        }
    }
};

#include "ict/core/expected.void.hpp"

#endif  // ICT_CORE_EXPECTED_HPP_

