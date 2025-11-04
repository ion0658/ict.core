#pragma once

#ifndef ICT_CORE_NET_IP_ADDR_V4_HPP
#define ICT_CORE_NET_IP_ADDR_V4_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <span>
#include <string_view>
#include <system_error>

#include "ict/core/expected.hpp"
#include "ict/core/trait/display.hpp"

#include "ict.core_export.h"

namespace ict::core::net {

struct ICT_CORE_EXPORT IpAddrV4 : public ict::core::IDisplay {
    constexpr IpAddrV4() noexcept : _bytes{0, 0, 0, 0} {}
    constexpr IpAddrV4(std::uint8_t byte_0, std::uint8_t byte_1, std::uint8_t byte_2, std::uint8_t byte_3) noexcept
        : _bytes{byte_0, byte_1, byte_2, byte_3} {}

    constexpr IpAddrV4(std::span<uint8_t, 4> bytes) noexcept : _bytes{bytes[0], bytes[1], bytes[2], bytes[3]} {}

    constexpr IpAddrV4(const IpAddrV4&) = default;
    constexpr IpAddrV4& operator=(const IpAddrV4&) = default;
    constexpr IpAddrV4(IpAddrV4&&) = default;
    constexpr IpAddrV4& operator=(IpAddrV4&&) = default;

    constexpr static IpAddrV4 loopback() noexcept { return IpAddrV4(127, 0, 0, 1); }
    constexpr static IpAddrV4 unspecified() noexcept { return IpAddrV4(0, 0, 0, 0); }
    constexpr static IpAddrV4 broadcast() noexcept { return IpAddrV4(255, 255, 255, 255); }

    static ict::core::expected<IpAddrV4, std::error_code> from(const std::string_view str);

    constexpr bool operator==(const IpAddrV4& other) const noexcept { return std::ranges::equal(_bytes, other._bytes); }
    constexpr IpAddrV4 operator|(const IpAddrV4& rhs) noexcept {
        return IpAddrV4(_bytes[0] | rhs._bytes[0], _bytes[1] | rhs._bytes[1], _bytes[2] | rhs._bytes[2],
                        _bytes[3] | rhs._bytes[3]);
    }
    constexpr IpAddrV4 operator&(const IpAddrV4& rhs) noexcept {
        return IpAddrV4(_bytes[0] & rhs._bytes[0], _bytes[1] & rhs._bytes[1], _bytes[2] & rhs._bytes[2],
                        _bytes[3] & rhs._bytes[3]);
    }

    virtual std::string display() const override;

   private:
    std::array<std::uint8_t, 4> _bytes;
};

struct ICT_CORE_EXPORT ip_addrv4_t {};                     // NOLINT
ICT_CORE_EXPORT inline constexpr ip_addrv4_t ip_addrv4{};  // NOLINT

}  // namespace ict::core::net

#endif  // ICT_CORE_NET_IP_ADDR_V4_HPP

