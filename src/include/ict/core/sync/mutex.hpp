#pragma once

#ifndef ICT_CORE_SYNC_MUTEX_HPP
#define ICT_CORE_SYNC_MUTEX_HPP

#include <chrono>
#include <cstdint>
#include <mutex>
#include <ratio>
#include <system_error>
#include <utility>

#include "ict/core/expected.hpp"
#include "ict/core/trait/not_copyable.hpp"

namespace ict::core::sync {

template <typename T, typename MutexT = std::mutex>
struct MutexGuard : private ict::core::INotCopyable {
    using reference = T&;
    using const_reference = const T&;

    MutexGuard() = delete;
    constexpr MutexGuard(reference ref, std::unique_lock<MutexT>&& lock) : _ref(ref), _lock(std::move(lock)) {}

    virtual ~MutexGuard() = default;

    MutexGuard(MutexGuard&&) = default;
    MutexGuard& operator=(MutexGuard&&) = default;

    constexpr reference as_ref() { return _ref; }
    constexpr const_reference as_cref() const { return _ref; }
    constexpr reference operator*() { return _ref; }
    constexpr const_reference operator*() const { return _ref; }

    constexpr std::unique_lock<MutexT>& ref_lock() { return _lock; }
    constexpr const std::unique_lock<MutexT>& ref_lock() const { return _lock; }

   private:
    reference _ref;
    std::unique_lock<MutexT> _lock;
};

template <typename T, typename MutexT = std::mutex>
struct Mutex : private ict::core::INotCopyable {
    Mutex() = default;
    constexpr Mutex(const T& value) : _value(value) {}
    constexpr Mutex(T&& value) : _value(std::move(value)) {}

    virtual ~Mutex() = default;

    Mutex(Mutex&&) = default;
    Mutex& operator=(Mutex&&) = default;

    constexpr MutexGuard<T, MutexT> lock() { return MutexGuard<T, MutexT>(_value, std::unique_lock<MutexT>(_mutex)); }

    constexpr ict::core::expected<MutexGuard<T, MutexT>, std::error_code> try_lock() {
        std::unique_lock<MutexT> ulock(_mutex, std::try_to_lock);
        if (ulock.owns_lock()) {
            return MutexGuard<T, MutexT>(_value, std::move(ulock));
        }
        return ict::core::unexpected(std::make_error_code(std::errc::resource_deadlock_would_occur));
    }

    template <typename Rep = int64_t, typename Period = std::ratio<1>>
    constexpr ict::core::expected<MutexGuard<T, MutexT>, std::error_code> try_lock_timeout(
        const std::chrono::duration<Rep, Period>& timeout_duration)
        requires requires(MutexT mtx) { mtx.try_lock_for(timeout_duration); }
    {
        std::unique_lock<MutexT> ulock(_mutex, std::defer_lock);
        if (ulock.try_lock_for(timeout_duration)) {
            return MutexGuard<T, MutexT>(_value, std::move(ulock));
        }
        return ict::core::unexpected(std::make_error_code(std::errc::timed_out));
    }

   private:
    T _value;
    MutexT _mutex;
};

}  // namespace ict::core::sync

#include "ict/core/sync/mutex.void.hpp"

#endif  // ICT_CORE_SYNC_MUTEX_HPP

