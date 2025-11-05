#include <format>

#include "ict/core/net/ip_addr.hpp"

std::string ict::core::net::IpAddress::display() const {
    if (is_v4()) {
        return std::format("IpAddrV4({})", this->get_v4());
    } else {
        return std::format("IpAddrV6({})", this->get_v6());
    }
}

