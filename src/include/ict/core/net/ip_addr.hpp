#pragma once

#ifndef ICT_CORE_NET_IP_ADDR_HPP
#define ICT_CORE_NET_IP_ADDR_HPP

#include <concepts>
#include <initializer_list>
#include <utility>
#include <variant>

#include "ict/core/net/ip_addr.v4.hpp"
#include "ict/core/net/ip_addr.v6.hpp"
#include "ict/core/trait/display.hpp"

#include "ict.core_export.h"

///
/// @brief ict::core::net - namespace for core networking components
///
namespace ict::core::net {

///
/// @brief IpAddress - class representing an IP address (either IpAddrV4 or IpAddrV6)
/// @details
/// struct IpAddress - a class that encapsulates an IP address, which can be either IpAddrV4 or IpAddrV6.
///
struct ICT_CORE_EXPORT IpAddress : public ict::core::IDisplay {
    using IpVariant = std::variant<IpAddrV4, IpAddrV6>;

    constexpr ~IpAddress() = default;
    ///
    /// @brief construct an IpAddress object with unspecified address (iv4)
    ///
    ///
    constexpr IpAddress() noexcept : _addr(IpAddrV4::unspecified()) {}
    ///
    /// @brief construct an IpAddress object with given address
    ///
    /// @param[in] addr ipv4 address
    ///
    constexpr IpAddress(const IpAddrV4& addr) noexcept : _addr(addr) {}
    ///
    /// @brief construct an IpAddress object with given address
    ///
    /// @param[in] addr ipv6 address
    ///
    constexpr IpAddress(const IpAddrV6& addr) noexcept : _addr(addr) {}

    constexpr IpAddress(const IpAddress&) = default;
    constexpr IpAddress& operator=(const IpAddress&) = default;

    constexpr IpAddress(IpAddress&&) = default;
    constexpr IpAddress& operator=(IpAddress&&) = default;

    template <typename T>
        requires(not ict::core::is_weak_same_as<std::in_place_t, T> && std::constructible_from<IpAddrV4, T>)
    constexpr IpAddress(T&& val) : _addr(IpAddrV4(std::forward<T>(val))) {}
    template <typename T>
        requires(not ict::core::is_weak_same_as<std::in_place_t, T> && std::constructible_from<IpAddrV6, T>)
    constexpr IpAddress(T&& val) : _addr(IpAddrV6(std::forward<T>(val))) {}

    template <class... Args>
        requires(std::constructible_from<IpAddrV4, Args...>)
    constexpr IpAddress(ict::core::net::ip_addrv4_t, Args&&... args) : _addr(IpAddrV4(std::forward<Args>(args)...)) {}
    template <class T, class... Args>
        requires(std::constructible_from<IpAddrV4, std::initializer_list<T>, Args...>)
    constexpr IpAddress(ict::core::net::ip_addrv4_t, std::initializer_list<T> ilist, Args&&... args)
        : _addr(IpAddrV4(ilist, std::forward<Args>(args)...)) {}

    template <class... Args>
        requires(std::constructible_from<IpAddrV6, Args...>)
    constexpr IpAddress(ict::core::net::ip_addrv6_t, Args&&... args) : _addr(IpAddrV6(std::forward<Args>(args)...)) {}
    template <class T, class... Args>
        requires(std::constructible_from<IpAddrV6, std::initializer_list<T>, Args...>)
    constexpr IpAddress(ict::core::net::ip_addrv6_t, std::initializer_list<T> ilist, Args&&... args)
        : _addr(IpAddrV6(ilist, std::forward<Args>(args)...)) {}

    constexpr IpAddress& operator=(const IpAddrV4& addr) noexcept {
        _addr = addr;
        return *this;
    }

    constexpr IpAddress& operator=(const IpAddrV6& addr) noexcept {
        _addr = addr;
        return *this;
    }

    ///
    /// @brief checks whether the address is ipv4
    ///
    /// @return bool true if the address is ipv4, false otherwise
    ///
    constexpr bool is_v4() const noexcept { return std::holds_alternative<IpAddrV4>(_addr); }
    ///
    /// @brief checks whether the address is ipv6
    ///
    /// @return bool true if the address is ipv6, false otherwise
    ///
    constexpr bool is_v6() const noexcept { return std::holds_alternative<IpAddrV6>(_addr); }

    ///
    /// @brief gets the ipv4 address
    ///
    /// @return reference to the ipv4 address
    ///
    /// @throws std::bad_variant_access if the address is not ipv4
    ///
    constexpr IpAddrV4& get_v4() { return std::get<IpAddrV4>(_addr); }
    ///
    /// @brief gets the ipv4 address
    ///
    /// @return reference to the ipv4 address
    ///
    /// @throws std::bad_variant_access if the address is not ipv4
    ///
    constexpr const IpAddrV4& get_v4() const { return std::get<IpAddrV4>(_addr); }
    ///
    /// @brief gets the ipv6 address
    ///
    /// @return reference to the ipv6 address
    ///
    /// @throws std::bad_variant_access if the address is not ipv6
    ///
    constexpr IpAddrV6& get_v6() { return std::get<IpAddrV6>(_addr); }
    ///
    /// @brief gets the ipv6 address
    ///
    /// @return reference to the ipv6 address
    ///
    /// @throws std::bad_variant_access if the address is not ipv6
    ///
    constexpr const IpAddrV6& get_v6() const { return std::get<IpAddrV6>(_addr); }

    constexpr bool operator==(const IpAddress& other) const noexcept { return _addr == other._addr; }

    virtual std::string display() const override;

   private:
    IpVariant _addr;
};

}  // namespace ict::core::net

#endif  // ICT_CORE_NET_IP_ADDR_HPP

