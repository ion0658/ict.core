#pragma once

#include <chrono>
#include <cstdint>
#include <mutex>
#include <ratio>
#include <system_error>
#include <utility>

#include "ict/core/expected.hpp"
#include "ict/core/sync/mutex.hpp"
#include "ict/core/trait/not_copyable.hpp"

namespace ict::core::sync {

template <typename MutexT>
struct MutexGuard<void, MutexT> : private ict::core::INotCopyable {
    MutexGuard() = delete;
    constexpr MutexGuard(std::unique_lock<MutexT>&& lock) : _lock(std::move(lock)) {}

    virtual ~MutexGuard() = default;

    MutexGuard(MutexGuard&&) = default;
    MutexGuard& operator=(MutexGuard&&) = default;

    constexpr std::unique_lock<MutexT>& ref_lock() { return _lock; }
    constexpr const std::unique_lock<MutexT>& ref_lock() const { return _lock; }

   private:
    std::unique_lock<MutexT> _lock;
};

template <typename MutexT>
struct Mutex<void, MutexT> : private ict::core::INotCopyable {
    Mutex() = default;

    virtual ~Mutex() = default;

    Mutex(Mutex&&) = default;
    Mutex& operator=(Mutex&&) = default;

    constexpr MutexGuard<void, MutexT> lock() { return MutexGuard<void, MutexT>(std::unique_lock<MutexT>(_mutex)); }

    constexpr ict::core::expected<MutexGuard<void, MutexT>, std::error_code> try_lock() {
        std::unique_lock<MutexT> ulock(_mutex, std::try_to_lock);
        if (ulock.owns_lock()) {
            return MutexGuard<void, MutexT>(std::move(ulock));
        }
        return ict::core::unexpected(std::make_error_code(std::errc::resource_deadlock_would_occur));
    }

    template <typename Rep = int64_t, typename Period = std::ratio<1>>
    constexpr ict::core::expected<MutexGuard<void, MutexT>, std::error_code> try_lock_timeout(
        const std::chrono::duration<Rep, Period>& timeout_duration)
        requires requires(MutexT mtx) { mtx.try_lock_for(timeout_duration); }
    {
        std::unique_lock<MutexT> ulock(_mutex, std::defer_lock);
        if (ulock.try_lock_for(timeout_duration)) {
            return MutexGuard<void, MutexT>(std::move(ulock));
        }
        return ict::core::unexpected(std::make_error_code(std::errc::timed_out));
    }

   private:
    MutexT _mutex;
};

}  // namespace ict::core::sync

