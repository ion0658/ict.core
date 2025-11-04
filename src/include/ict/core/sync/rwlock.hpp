#pragma once

#ifndef ICT_CORE_SYNC_RWLOCK_HPP
#define ICT_CORE_SYNC_RWLOCK_HPP

#include <mutex>
#include <shared_mutex>
#include <system_error>

#include "ict/core/expected.hpp"
#include "ict/core/trait/not_copyable.hpp"

namespace ict::core::sync {

template <typename T>
struct ReadLockGuard : private ict::core::INotCopyable {
    using const_reference = const T&;

    ReadLockGuard() = delete;
    ReadLockGuard(const_reference ref, std::shared_lock<std::shared_mutex>&& lock)
        : _ref(ref), _lock(std::move(lock)) {}

    virtual ~ReadLockGuard() = default;

    ReadLockGuard(ReadLockGuard&&) = default;
    ReadLockGuard& operator=(ReadLockGuard&&) = default;

    constexpr const_reference as_ref() const { return _ref; }
    constexpr const_reference operator*() const { return _ref; }

    constexpr std::shared_lock<std::shared_mutex>& ref_lock() { return _lock; }
    constexpr const std::shared_lock<std::shared_mutex>& ref_lock() const { return _lock; }

   private:
    const_reference _ref;
    std::shared_lock<std::shared_mutex> _lock;
};

template <typename T>
struct WriteLockGuard : private ict::core::INotCopyable {
    using reference = T&;

    WriteLockGuard() = delete;
    WriteLockGuard(reference ref, std::unique_lock<std::shared_mutex> lock) : _ref(ref), _lock(std::move(lock)) {}

    virtual ~WriteLockGuard() = default;

    WriteLockGuard(WriteLockGuard&&) = default;
    WriteLockGuard& operator=(WriteLockGuard&&) = default;

    constexpr reference as_ref() { return _ref; }
    constexpr reference operator*() { return _ref; }

    constexpr std::unique_lock<std::shared_mutex>& ref_lock() { return _lock; }
    constexpr const std::unique_lock<std::shared_mutex>& ref_lock() const { return _lock; }

   private:
    reference _ref;
    std::unique_lock<std::shared_mutex> _lock;
};

template <typename T>
struct RWLock : private ict::core::INotCopyable {
    RWLock() = default;
    constexpr RWLock(const T& value) : _value(value) {}
    constexpr RWLock(T&& value) : _value(std::move(value)) {}

    RWLock(RWLock&&) = default;
    RWLock& operator=(RWLock&&) = default;

    constexpr ReadLockGuard<T> read() { return ReadLockGuard<T>(_value, std::shared_lock<std::shared_mutex>(_mutex)); }
    ict::core::expected<ReadLockGuard<T>, std::error_code> try_read() {
        std::shared_lock<std::shared_mutex> lock(_mutex, std::try_to_lock);
        if (!lock.owns_lock()) {
            return ict::core::unexpected(std::make_error_code(std::errc::resource_deadlock_would_occur));
        }
        return ReadLockGuard<T>(_value, std::move(lock));
    }

    constexpr WriteLockGuard<T> write() {
        return WriteLockGuard<T>(_value, std::unique_lock<std::shared_mutex>(_mutex));
    }
    ict::core::expected<WriteLockGuard<T>, std::error_code> try_write() {
        std::unique_lock<std::shared_mutex> lock(_mutex, std::try_to_lock);
        if (!lock.owns_lock()) {
            return ict::core::unexpected(std::make_error_code(std::errc::resource_deadlock_would_occur));
        }
        return WriteLockGuard<T>(_value, std::move(lock));
    }

   private:
    T _value;
    std::shared_mutex _mutex;
};

}  // namespace ict::core::sync

#endif  // ICT_CORE_SYNC_RWLOCK_HPP

