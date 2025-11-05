#include <memory>
#include <thread>

#include "gtest/gtest.h"

#include "ict/core/sync/condvar.hpp"
#include "ict/core/sync/mutex.hpp"
#include "ict/core/sync/rwlock.hpp"

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

TEST(mutex, rwlock) {
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

TEST(mutex, condvar_1) {
    auto mtx = std::make_shared<ict::core::sync::Mutex<bool>>(false);
    auto condvar = std::make_shared<ict::core::sync::CondVar>();

    auto wait_worker = [](std::shared_ptr<ict::core::sync::Mutex<bool>> mtx,
                          std::shared_ptr<ict::core::sync::CondVar> condvar) {
        auto guard = mtx->lock();
        while (!*guard) {
            condvar->wait(guard);
        }
    };
    auto notify_worker = [](std::shared_ptr<ict::core::sync::Mutex<bool>> mtx,
                            std::shared_ptr<ict::core::sync::CondVar> condvar) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto guard = mtx->lock();
        *guard = true;
        condvar->notify_one();
    };
    auto c_mtx = mtx;
    auto c_condvar = condvar;
    std::thread waiter(wait_worker, std::move(c_mtx), std::move(c_condvar));
    notify_worker(mtx, condvar);
    waiter.join();

    EXPECT_TRUE(*mtx->lock());
}

TEST(mutex, condvar_2) {
    auto mtx = std::make_shared<ict::core::sync::Mutex<bool>>(false);
    auto condvar = std::make_shared<ict::core::sync::CondVar>();

    auto wait_worker = [](std::shared_ptr<ict::core::sync::Mutex<bool>> mtx,
                          std::shared_ptr<ict::core::sync::CondVar> condvar) {
        auto guard = mtx->lock();
        condvar->wait_while(guard, [&guard]() { return *guard; });
    };
    auto notify_worker = [](std::shared_ptr<ict::core::sync::Mutex<bool>> mtx,
                            std::shared_ptr<ict::core::sync::CondVar> condvar) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            auto guard = mtx->lock();
            *guard = true;
        }
        condvar->notify_one();
    };
    auto c_mtx = mtx;
    auto c_condvar = condvar;
    std::thread waiter(wait_worker, std::move(c_mtx), std::move(c_condvar));
    notify_worker(mtx, condvar);
    waiter.join();

    EXPECT_TRUE(*mtx->lock());
}

