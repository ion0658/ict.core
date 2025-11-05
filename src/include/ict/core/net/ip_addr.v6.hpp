#pragma once

#ifndef ICT_CORE_NET_IP_ADDR_V6_HPP
#define ICT_CORE_NET_IP_ADDR_V6_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <system_error>

#include "ict/core/expected.hpp"
#include "ict/core/trait/display.hpp"

#include "ict.core_export.h"

namespace ict::core::net {

///
/// @brief IpAddrV6 - class representing an IPv6 address
///
///
struct ICT_CORE_EXPORT IpAddrV6 : public ict::core::IDisplay {
    ///
    /// @brief ipv6 default constructor
    ///
    ///
    constexpr IpAddrV6() noexcept : _bytes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}
    ///
    /// @brief ipv6 constructor from 16 bytes
    ///
    /// @param[in] byte_0 byte 0
    /// @param[in] byte_1 byte 1
    /// @param[in] byte_2 byte 2
    /// @param[in] byte_3 byte 3
    /// @param[in] byte_4 byte 4
    /// @param[in] byte_5 byte 5
    /// @param[in] byte_6 byte 6
    /// @param[in] byte_7 byte 7
    /// @param[in] byte_8 byte 8
    /// @param[in] byte_9 byte 9
    /// @param[in] byte_10 byte 10
    /// @param[in] byte_11 byte 11
    /// @param[in] byte_12 byte 12
    /// @param[in] byte_13 byte 13
    /// @param[in] byte_14 byte 14
    /// @param[in] byte_15 byte 15
    ///
    constexpr IpAddrV6(std::uint8_t byte_0,
                       std::uint8_t byte_1,
                       std::uint8_t byte_2,
                       std::uint8_t byte_3,
                       std::uint8_t byte_4,
                       std::uint8_t byte_5,
                       std::uint8_t byte_6,
                       std::uint8_t byte_7,
                       std::uint8_t byte_8,
                       std::uint8_t byte_9,
                       std::uint8_t byte_10,
                       std::uint8_t byte_11,
                       std::uint8_t byte_12,
                       std::uint8_t byte_13,
                       std::uint8_t byte_14,
                       std::uint8_t byte_15) noexcept
        : _bytes{byte_0, byte_1, byte_2,  byte_3,  byte_4,  byte_5,  byte_6,  byte_7,
                 byte_8, byte_9, byte_10, byte_11, byte_12, byte_13, byte_14, byte_15} {}

    ///
    /// @brief ipv6 constructor from byte span
    ///
    /// @param[in] bytes byte span
    ///
    constexpr IpAddrV6(const std::span<uint8_t, 16> bytes) noexcept
        : _bytes{bytes[0], bytes[1], bytes[2],  bytes[3],  bytes[4],  bytes[5],  bytes[6],  bytes[7],
                 bytes[8], bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]} {}

    constexpr IpAddrV6(const IpAddrV6&) = default;
    constexpr IpAddrV6& operator=(const IpAddrV6&) = default;
    constexpr IpAddrV6(IpAddrV6&&) = default;
    constexpr IpAddrV6& operator=(IpAddrV6&&) = default;

    ///
    /// @brief ipv6 local loopback address
    ///
    /// @return ipv6 loopback address(::1)
    ///
    constexpr static IpAddrV6 loopback() noexcept { return IpAddrV6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1); }
    ///
    /// @brief ipv6 unspecified address
    ///
    /// @return ipv6 unspecified address(::)
    ///
    constexpr static IpAddrV6 unspecified() noexcept {
        return IpAddrV6(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    ///
    /// @brief try to parse ipv6 address from string
    ///
    /// @param[in] str string representation of ipv6 address
    ///
    /// @return value: ipv6 address object if parsing succeeded, invalid_argument error code otherwise
    ///
    static ict::core::expected<IpAddrV6, std::error_code> from(const std::string_view str);

    constexpr bool operator==(const IpAddrV6& other) const noexcept { return std::ranges::equal(_bytes, other._bytes); }
    constexpr bool operator!=(const IpAddrV6& other) const noexcept { return !(*this == other); }

    ///
    /// @brief bit mask OR operator
    ///
    /// @param[in] rhs other ipv6 address
    /// @return bitwise ORed ipv6 address
    ///
    constexpr IpAddrV6 operator|(const IpAddrV6& rhs) noexcept {
        return IpAddrV6(_bytes[0] | rhs._bytes[0], _bytes[1] | rhs._bytes[1], _bytes[2] | rhs._bytes[2],
                        _bytes[3] | rhs._bytes[3], _bytes[4] | rhs._bytes[4], _bytes[5] | rhs._bytes[5],
                        _bytes[6] | rhs._bytes[6], _bytes[7] | rhs._bytes[7], _bytes[8] | rhs._bytes[8],
                        _bytes[9] | rhs._bytes[9], _bytes[10] | rhs._bytes[10], _bytes[11] | rhs._bytes[11],
                        _bytes[12] | rhs._bytes[12], _bytes[13] | rhs._bytes[13], _bytes[14] | rhs._bytes[14],
                        _bytes[15] | rhs._bytes[15]);
    }
    ///
    /// @brief bit mask AND operator
    ///
    /// @param[in] rhs other ipv6 address
    /// @return bitwise ANDed ipv6 address
    ///
    constexpr IpAddrV6 operator&(const IpAddrV6& rhs) noexcept {
        return IpAddrV6(_bytes[0] & rhs._bytes[0], _bytes[1] & rhs._bytes[1], _bytes[2] & rhs._bytes[2],
                        _bytes[3] & rhs._bytes[3], _bytes[4] & rhs._bytes[4], _bytes[5] & rhs._bytes[5],
                        _bytes[6] & rhs._bytes[6], _bytes[7] & rhs._bytes[7], _bytes[8] & rhs._bytes[8],
                        _bytes[9] & rhs._bytes[9], _bytes[10] & rhs._bytes[10], _bytes[11] & rhs._bytes[11],
                        _bytes[12] & rhs._bytes[12], _bytes[13] & rhs._bytes[13], _bytes[14] & rhs._bytes[14],
                        _bytes[15] & rhs._bytes[15]);
    }

    ///
    /// @brief bitwise NOT operator
    ///
    /// @return bitwise NOTed ipv6 address
    ///
    constexpr IpAddrV6 operator~() const noexcept {
        return IpAddrV6(~_bytes[0], ~_bytes[1], ~_bytes[2], ~_bytes[3], ~_bytes[4], ~_bytes[5], ~_bytes[6], ~_bytes[7],
                        ~_bytes[8], ~_bytes[9], ~_bytes[10], ~_bytes[11], ~_bytes[12], ~_bytes[13], ~_bytes[14],
                        ~_bytes[15]);
    }

    virtual std::string display() const override;

   private:
    std::array<std::uint8_t, 16> _bytes;
};

struct ICT_CORE_EXPORT ip_addrv6_t {};     // NOLINT
inline constexpr ip_addrv6_t ip_addrv6{};  // NOLINT

}  // namespace ict::core::net

#endif  // ICT_CORE_NET_IP_ADDR_V6_HPP

