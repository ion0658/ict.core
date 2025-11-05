#include "gtest/gtest.h"

#include "ict/core/net/ip_addr.v4.hpp"

TEST(ip_addr_v4, construction) {
    auto unspecified = ict::core::net::IpAddrV4::unspecified();
    auto loopback = ict::core::net::IpAddrV4::loopback();
    auto broadcast = ict::core::net::IpAddrV4::broadcast();
    auto from_bytes = ict::core::net::IpAddrV4(192, 168, 1, 100);
    auto from_span = ict::core::net::IpAddrV4({192, 168, 1, 200});

    EXPECT_EQ(unspecified, ict::core::net::IpAddrV4(0, 0, 0, 0));
    EXPECT_EQ(loopback, ict::core::net::IpAddrV4(127, 0, 0, 1));
    EXPECT_EQ(broadcast, ict::core::net::IpAddrV4(255, 255, 255, 255));
}

TEST(ip_addr_v4, parse_str) {
    auto ip2_res = ict::core::net::IpAddrV4::from("192.168.1.100");
    EXPECT_TRUE(ip2_res.has_value());
    auto ip2 = ip2_res.value();
    EXPECT_EQ(ip2, ict::core::net::IpAddrV4(192, 168, 1, 100));
    auto err_res = ict::core::net::IpAddrV4::from("999.999.999.999");
    EXPECT_FALSE(err_res.has_value());
}

TEST(ip_addr_v4, to_string) {
    auto ip1 = ict::core::net::IpAddrV4(10, 0, 0, 1);
    EXPECT_EQ(std::format("{}", ip1), "10.0.0.1");
}

TEST(ip_addr_v4, comparison) {
    auto ip1 = ict::core::net::IpAddrV4(192, 168, 1, 100);
    auto ip2 = ict::core::net::IpAddrV4(192, 168, 1, 100);
    auto ip3 = ict::core::net::IpAddrV4(10, 0, 0, 1);

    EXPECT_EQ(ip1, ip2);
    EXPECT_NE(ip1, ip3);
}

TEST(ip_addr_v4, mask) {
    auto ip1 = ict::core::net::IpAddrV4(192, 168, 1, 150);
    auto mask = ict::core::net::IpAddrV4(255, 255, 255, 0);
    auto network = ip1 & mask;
    EXPECT_EQ(network, ict::core::net::IpAddrV4(192, 168, 1, 0));
    auto host = ip1 & ~mask;
    EXPECT_EQ(host, ict::core::net::IpAddrV4(0, 0, 0, 150));
    auto ip2 = network | host;
    EXPECT_EQ(ip2, ip1);
}

