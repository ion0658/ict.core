#pragma once

#ifndef ICT_CORE_NET_SOCKADDR_HPP
#define ICT_CORE_NET_SOCKADDR_HPP

#include "ict/core/net/ip_addr.hpp"
#include "ict/core/trait/display.hpp"

#include "ict.core_export.h"

namespace ict::core::net {

struct ICT_CORE_EXPORT SockAddr : public ict::core::IDisplay {
    constexpr ~SockAddr() = default;
    constexpr SockAddr() noexcept : _ip_addr(IpAddrV4::unspecified()), _port(0) {}
    constexpr SockAddr(const ict::core::net::IpAddress& ip_addr, uint16_t port) noexcept
        : _ip_addr(ip_addr), _port(port) {}

    constexpr SockAddr(const SockAddr&) = default;
    constexpr SockAddr& operator=(const SockAddr&) = default;
    constexpr SockAddr(SockAddr&&) = default;
    constexpr SockAddr& operator=(SockAddr&&) = default;

    constexpr const ict::core::net::IpAddress& ip() const noexcept { return _ip_addr; }
    constexpr uint16_t port() const noexcept { return _port; }

    constexpr ict::core::net::SockAddr& set_port(uint16_t port) noexcept {
        _port = port;
        return *this;
    }
    constexpr ict::core::net::SockAddr& set_ip(const ict::core::net::IpAddress& ip_addr) noexcept {
        _ip_addr = ip_addr;
        return *this;
    }

    virtual std::string display() const override;

   private:
    ict::core::net::IpAddress _ip_addr;
    uint16_t _port;
};
}  // namespace ict::core::net

#endif  // ICT_CORE_NET_SOCKADDR_HPP

