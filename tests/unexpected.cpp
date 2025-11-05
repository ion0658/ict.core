#include <format>

#include "gtest/gtest.h"

#include "ict/core/unexpected.hpp"

TEST(unexpected, construction) {
    ict::core::unexpected<int> err1(10);
    EXPECT_EQ(err1.error(), 10);

    ict::core::unexpected<std::string> err2("failure");
    EXPECT_EQ(err2.error(), "failure");
}

TEST(unexpected, make_unexpected) {
    auto err = ict::core::make_unexpected(42);
    EXPECT_EQ(err.error(), 42);
}

TEST(unexpected, copy_move) {
    auto err1 = ict::core::make_unexpected(std::string("error"));
    auto err2 = err1;  // copy
    EXPECT_EQ(err2.error(), "error");

    auto err3 = std::move(err1);  // move
    EXPECT_EQ(err3.error(), "error");
}

TEST(unexpected, comparison) {
    auto err1 = ict::core::make_unexpected(100);
    auto err2 = ict::core::make_unexpected(100);
    auto err3 = ict::core::make_unexpected(200);

    EXPECT_EQ(err1, err2);
    EXPECT_NE(err1, err3);
}

TEST(unexpected, formatting) {
    auto err = ict::core::make_unexpected(3.14);
    EXPECT_EQ(std::format("{}", err), "3.14");
}

