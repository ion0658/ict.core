#pragma once

#ifndef ICT_CORE_SYNC_RWLOCK_HPP
#define ICT_CORE_SYNC_RWLOCK_HPP

#include <mutex>
#include <shared_mutex>
#include <system_error>

#include "ict/core/expected.hpp"
#include "ict/core/trait/not_copyable.hpp"

namespace ict::core::sync {

///
/// @brief ReadLockGuard - A guard that provides read-only access to a data of type T while holding a shared lock.
/// @details The ReadLockGuard holds a shared lock on a std::shared_mutex, allowing multiple concurrent readers.
///
/// @tparam T Data type to be accessed through the ReadLockGuard.
///
template <typename T>
struct ReadLockGuard : private ict::core::INotCopyable {
    using const_reference = const T&;

    ReadLockGuard() = delete;
    ReadLockGuard(const_reference ref, std::shared_lock<std::shared_mutex>&& lock)
        : _ref(ref), _lock(std::move(lock)) {}

    virtual ~ReadLockGuard() = default;

    ReadLockGuard(ReadLockGuard&&) = default;
    ReadLockGuard& operator=(ReadLockGuard&&) = default;

    ///
    /// @brief returns a const reference to the protected data.
    ///
    /// @return ref T A const reference to the protected data.
    ///
    constexpr const_reference as_ref() const { return _ref; }

    ///
    /// @brief returns a const reference to the protected data.
    ///
    /// @return ref T A const reference to the protected data.
    ///
    constexpr const_reference operator*() const { return _ref; }

    ///
    /// @brief gets a reference to the underlying shared lock.
    ///
    /// @return std::shared_lock<std::shared_mutex>& A reference to the underlying shared lock.
    ///
    constexpr std::shared_lock<std::shared_mutex>& ref_lock() { return _lock; }
    ///
    /// @brief gets a const reference to the underlying shared lock.
    ///
    /// @return const std::shared_lock<std::shared_mutex>& A const reference to the underlying shared lock.
    ///
    constexpr const std::shared_lock<std::shared_mutex>& ref_lock() const { return _lock; }

   private:
    const_reference _ref;
    std::shared_lock<std::shared_mutex> _lock;
};

///
/// @brief WriteLockGuard - A guard that provides mutable access to a data of type T while holding an exclusive lock.
/// @details The WriteLockGuard holds an exclusive lock on a std::shared_mutex, allowing only one writer at a time.
///
/// @tparam T Data type to be accessed through the WriteLockGuard.
///
template <typename T>
struct WriteLockGuard : private ict::core::INotCopyable {
    using reference = T&;

    WriteLockGuard() = delete;
    WriteLockGuard(reference ref, std::unique_lock<std::shared_mutex> lock) : _ref(ref), _lock(std::move(lock)) {}

    virtual ~WriteLockGuard() = default;

    WriteLockGuard(WriteLockGuard&&) = default;
    WriteLockGuard& operator=(WriteLockGuard&&) = default;

    ///
    /// @brief gets a reference to the protected data.
    ///
    /// @return ref T A reference to the protected data.
    ///
    constexpr reference as_ref() { return _ref; }
    ///
    /// @brief gets a reference to the protected data.
    ///
    /// @return ref T A reference to the protected data.
    ///
    constexpr reference operator*() { return _ref; }

    ///
    /// @brief gets a reference to the underlying unique lock.
    ///
    /// @return std::unique_lock<std::shared_mutex>& A reference to the underlying unique lock.
    ///
    constexpr std::unique_lock<std::shared_mutex>& ref_lock() { return _lock; }
    ///
    /// @brief gets a const reference to the underlying unique lock.
    ///
    /// @return const std::unique_lock<std::shared_mutex>& A const reference to the underlying unique lock.
    ///
    constexpr const std::unique_lock<std::shared_mutex>& ref_lock() const { return _lock; }

   private:
    reference _ref;
    std::unique_lock<std::shared_mutex> _lock;
};

///
/// @brief RWLock - A read-write lock that provides synchronized access to a data of type T.
/// @details The RWLock allows multiple concurrent readers or a single writer at any given time.
///
/// @tparam T Data type to be protected by the RWLock.
///
template <typename T>
struct RWLock : private ict::core::INotCopyable {
    RWLock() = default;
    constexpr RWLock(const T& value) : _value(value) {}
    constexpr RWLock(T&& value) : _value(std::move(value)) {}

    RWLock(RWLock&&) = default;
    RWLock& operator=(RWLock&&) = default;

    ///
    /// @brief get a read lock guard for the protected data.
    ///
    /// @return ReadLockGuard<T> A read lock guard that provides read-only access to the protected data.
    ///
    constexpr ReadLockGuard<T> read() { return ReadLockGuard<T>(_value, std::shared_lock<std::shared_mutex>(_mutex)); }
    ict::core::expected<ReadLockGuard<T>, std::error_code> try_read() {
        std::shared_lock<std::shared_mutex> lock(_mutex, std::try_to_lock);
        if (!lock.owns_lock()) {
            return ict::core::unexpected(std::make_error_code(std::errc::resource_deadlock_would_occur));
        }
        return ReadLockGuard<T>(_value, std::move(lock));
    }

    ///
    /// @brief get a write lock guard for the protected data.
    ///
    /// @return WriteLockGuard<T> A write lock guard that provides mutable access to the protected data.
    ///
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

