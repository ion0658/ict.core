#include "gtest/gtest.h"

#include "ict/core/sync/condvar.hpp"
#include "ict/core/sync/mutex.hpp"
#include "ict/core/sync/rwlock.hpp"

TEST(rwlock, rwlock) {
    auto rwlock = ict::core::sync::RWLock<int>{42};
    {
        auto read_guard1 = rwlock.read();
        EXPECT_EQ(*read_guard1, 42);
        auto read_guard2 = rwlock.read();
        EXPECT_EQ(*read_guard2, 42);
        SUCCEED();
    }
    {
        auto write_guard = rwlock.write();
        EXPECT_EQ(*write_guard, 42);
        *write_guard = 100;
        EXPECT_EQ(*write_guard, 100);
    }
}

TEST(rwlock, try_lock) {
    auto rwlock = ict::core::sync::RWLock<int>{42};
    {
        auto read_guard1 = rwlock.read();
        EXPECT_EQ(*read_guard1, 42);
        auto read_guard2 = rwlock.read();
        EXPECT_EQ(*read_guard2, 42);
        SUCCEED();
    }
    {
        auto write_guard = rwlock.write();
        EXPECT_EQ(*write_guard, 42);
        *write_guard = 100;
        EXPECT_EQ(*write_guard, 100);
        auto try_read_guard = rwlock.try_read();
        auto try_write_guard = rwlock.try_write();
        EXPECT_FALSE(try_read_guard.has_value());
        EXPECT_FALSE(try_write_guard.has_value());
    }
}

