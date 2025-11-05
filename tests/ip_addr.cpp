#include "gtest/gtest.h"

#include "ict/core/net/ip_addr.hpp"

TEST(ip_address, construction) {
    auto ipv4 = ict::core::net::IpAddrV4::loopback();
    auto ipv6 = ict::core::net::IpAddrV6::loopback();
    auto ip_addr_v4 = ict::core::net::IpAddress(ipv4);
    auto ip_addr_v6 = ict::core::net::IpAddress(ipv6);
    EXPECT_TRUE(ip_addr_v4.is_v4());
    EXPECT_TRUE(ip_addr_v6.is_v6());
}

TEST(ip_address, value_access) {
    auto ipv4 = ict::core::net::IpAddrV4::loopback();
    auto ipv6 = ict::core::net::IpAddrV6::loopback();
    auto ip_addr_v4 = ict::core::net::IpAddress(ipv4);
    auto ip_addr_v6 = ict::core::net::IpAddress(ipv6);

    EXPECT_EQ(ip_addr_v4.get_v4(), ipv4);
    EXPECT_EQ(ip_addr_v6.get_v6(), ipv6);
}

TEST(ip_address, to_string) {
    auto ipv4 = ict::core::net::IpAddrV4::loopback();
    auto ipv6 = ict::core::net::IpAddrV6::loopback();
    auto ip_addr_v4 = ict::core::net::IpAddress(ipv4);
    auto ip_addr_v6 = ict::core::net::IpAddress(ipv6);

    EXPECT_EQ(std::format("{}", ip_addr_v4), std::format("IpAddrV4({})", ipv4));
    EXPECT_EQ(std::format("{}", ip_addr_v6), std::format("IpAddrV6({})", ipv6));
}

