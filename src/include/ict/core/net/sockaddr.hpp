#pragma once

#ifndef ICT_CORE_NET_SOCKADDR_HPP
#define ICT_CORE_NET_SOCKADDR_HPP

#include "ict/core/net/ip_addr.hpp"
#include "ict/core/trait/display.hpp"

#include "ict.core_export.h"

namespace ict::core::net {

///
/// @brief SockAddr - Socket Address structure representing an IP Address and port number.
///
///
struct ICT_CORE_EXPORT SockAddr : public ict::core::IDisplay {
    constexpr ~SockAddr() = default;
    ///
    /// @brief default constructor initializing to unspecified IP and port 0.
    ///
    ///
    constexpr SockAddr() noexcept : _ip_addr(IpAddrV4::unspecified()), _port(0) {}
    ///
    /// @brief constructor initializing with given IP address and port number.
    ///
    /// @param[in] ip_addr ip address
    /// @param[in] port port number
    ///
    /// @returns SockAddr instance
    ///
    constexpr SockAddr(const ict::core::net::IpAddress& ip_addr, uint16_t port) noexcept
        : _ip_addr(ip_addr), _port(port) {}

    constexpr SockAddr(const SockAddr&) = default;
    constexpr SockAddr& operator=(const SockAddr&) = default;
    constexpr SockAddr(SockAddr&&) = default;
    constexpr SockAddr& operator=(SockAddr&&) = default;

    ///
    /// @brief gets the IP address.
    ///
    /// @return ip address
    ///
    constexpr const ict::core::net::IpAddress& ip() const noexcept { return _ip_addr; }
    ///
    /// @brief gets the port number.
    ///
    /// @return uint16_t port number
    ///
    constexpr uint16_t port() const noexcept { return _port; }

    ///
    /// @brief sets the IP address.
    ///
    /// @param[in] ip_addr ip address
    ///
    /// @returns reference to SockAddr
    ///
    constexpr ict::core::net::SockAddr& set_ip(const ict::core::net::IpAddress& ip_addr) noexcept {
        _ip_addr = ip_addr;
        return *this;
    }
    ///
    /// @brief sets the port number.
    ///
    /// @param[in] port port number
    ///
    /// @returns reference to SockAddr
    ///
    constexpr ict::core::net::SockAddr& set_port(uint16_t port) noexcept {
        _port = port;
        return *this;
    }

    virtual std::string display() const override;

   private:
    ict::core::net::IpAddress _ip_addr;
    uint16_t _port;
};
}  // namespace ict::core::net

#endif  // ICT_CORE_NET_SOCKADDR_HPP

