#include "ict/core/net/ip_addr.hpp"

std::string ict::core::net::IpAddress::display() const {
    if (is_v4()) {
        return get_v4().display();
    } else {
        return get_v6().display();
    }
}

