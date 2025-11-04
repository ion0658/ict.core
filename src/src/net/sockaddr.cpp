#include "ict/core/net/sockaddr.hpp"

std::string ict::core::net::SockAddr::display() const {
    return std::format("{}:{}", this->_ip_addr, this->_port);
}

