#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <system_error>

#include "ict/core/expected.hpp"
#include "ict/core/net/ip_addr.v6.hpp"

#if defined(_WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

ict::core::expected<ict::core::net::IpAddrV6, std::error_code> ict::core::net::IpAddrV6::from(
    const std::string_view str) {
    sockaddr_in6 addr{};
    if (!inet_pton(AF_INET6, str.data(), &addr.sin6_addr)) {
        return ict::core::unexpected(std::make_error_code(std::errc::invalid_argument));
    }
    auto ipv6 = ict::core::net::IpAddrV6();
    std::copy_n(reinterpret_cast<uint8_t*>(&addr.sin6_addr), ipv6._bytes.size(), ipv6._bytes.data());
    return ipv6;
}

std::string ict::core::net::IpAddrV6::display() const {
    sockaddr_in6 addr{};
    std::copy_n(this->_bytes.data(), this->_bytes.size(), reinterpret_cast<uint8_t*>(&addr.sin6_addr));
    char addr_str[INET6_ADDRSTRLEN] = {};
    inet_ntop(AF_INET6, &addr.sin6_addr, addr_str, sizeof(addr_str));
    return addr_str;
}

