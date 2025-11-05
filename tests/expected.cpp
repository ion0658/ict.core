#include <format>

#include "gtest/gtest.h"

#include "ict/core/expected.hpp"

TEST(expected, construction) {
    ict::core::expected<int, std::string> res1(42);
    EXPECT_TRUE(res1.has_value());
    EXPECT_EQ(res1.value(), 42);

    ict::core::expected<int, std::string> res2 = ict::core::make_unexpected<std::string>("failure");
    EXPECT_FALSE(res2.has_value());
    EXPECT_EQ(res2.error(), "failure");
}

TEST(expected, copy_move) {
    ict::core::expected<std::string, int> res1("success");
    auto res2 = res1;  // copy
    EXPECT_TRUE(res2.has_value());
    EXPECT_EQ(res2.value(), "success");

    auto res3 = std::move(res1);  // move
    EXPECT_TRUE(res3.has_value());
    EXPECT_EQ(res3.value(), "success");
}

TEST(expected, comparison) {
    ict::core::expected<int, std::string> res1(10);
    ict::core::expected<int, std::string> res2(10);
    ict::core::expected<int, std::string> res3 = ict::core::make_unexpected<std::string>("error");
    ict::core::expected<int, std::string> res4(20);
    ict::core::expected<int, std::string> res5 = ict::core::make_unexpected<std::string>("different error");

    EXPECT_EQ(res1, 10);
    EXPECT_EQ(res1, res2);
    EXPECT_NE(res1, res3);
    EXPECT_NE(res1, res4);
    EXPECT_EQ(res3, ict::core::make_unexpected<std::string>("error"));
    EXPECT_NE(res3, res5);
}

TEST(expected, formatting) {
    ict::core::expected<int, std::string> res1(42);
    EXPECT_EQ(std::format("{}", res1), "Ok(42)");

    ict::core::expected<int, std::string> res2 = ict::core::make_unexpected<std::string>("failure");
    EXPECT_EQ(std::format("{}", res2), "Err(failure)");
}

TEST(expected_void, construction) {
    ict::core::expected<void, std::string> res1;
    EXPECT_TRUE(res1.has_value());

    ict::core::expected<void, std::string> res2 = ict::core::make_unexpected<std::string>("failure");
    EXPECT_FALSE(res2.has_value());
    EXPECT_EQ(res2.error(), "failure");
}

TEST(expected_void, copy_move) {
    ict::core::expected<void, int> res1;
    auto res2 = res1;  // copy
    EXPECT_TRUE(res2.has_value());

    auto res3 = std::move(res1);  // move
    EXPECT_TRUE(res3.has_value());
}

TEST(expected_void, comparison) {
    ict::core::expected<void, std::string> res1;
    ict::core::expected<void, std::string> res2;
    ict::core::expected<void, std::string> res3 = ict::core::make_unexpected<std::string>("error");

    EXPECT_EQ(res1, res2);
    EXPECT_NE(res1, res3);
    EXPECT_EQ(res3, ict::core::make_unexpected<std::string>("error"));
}

TEST(expected_void, formatting) {
    ict::core::expected<void, std::string> res1;
    EXPECT_EQ(std::format("{}", res1), "Ok(())");

    ict::core::expected<void, std::string> res2 = ict::core::make_unexpected<std::string>("failure");
    EXPECT_EQ(std::format("{}", res2), "Err(failure)");
}

