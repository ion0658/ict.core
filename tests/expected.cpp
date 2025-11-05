#include "gtest/gtest.h"

#include "ict/core/expected.hpp"

TEST(unexpected, make_unexpected) {
    auto err = ict::core::make_unexpected(42);
    EXPECT_EQ(err.error(), 42);
}

