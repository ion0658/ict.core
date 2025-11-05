#include "gtest/gtest.h"

#include "ict/core/sync/mutex.hpp"

TEST(mutex, lock_void) {
    auto mtx = ict::core::sync::Mutex<void>{};
    auto guard = mtx.lock();
    SUCCEED();
}

TEST(mutex, lock_int) {
    auto mtx = ict::core::sync::Mutex<int>{42};
    auto guard = mtx.lock();
    EXPECT_EQ(*guard, 42);
    *guard = 100;
    EXPECT_EQ(*guard, 100);
}

TEST(mutex, try_lock) {
    auto mtx = ict::core::sync::Mutex<int>{42};
    auto guard1 = mtx.lock();
    auto guard2 = mtx.try_lock();
    EXPECT_EQ(guard2.has_value(), false);
    EXPECT_EQ(*guard1, 42);
}

