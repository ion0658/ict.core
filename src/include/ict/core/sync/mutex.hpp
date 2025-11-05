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

///
/// @brief ict::core::sync - Synchronization primitives
///
namespace ict::core::sync {

///
/// @brief MutexGuard - Mutex guard for protected data
/// @details
/// struct MutexGuard - provides access to the data protected by mutex.
///
/// @tparam T value type
/// @tparam MutexT mutex type (default: std::mutex)
///
template <typename T, typename MutexT = std::mutex>
struct MutexGuard : private ict::core::INotCopyable {
    using reference = T&;
    using const_reference = const T&;

    MutexGuard() = delete;
    constexpr MutexGuard(reference ref, std::unique_lock<MutexT>&& lock) : _ref(ref), _lock(std::move(lock)) {}

    virtual ~MutexGuard() = default;

    MutexGuard(MutexGuard&&) = default;
    MutexGuard& operator=(MutexGuard&&) = default;

    ///
    /// @brief returns the reference to the protected data
    ///
    /// @return reference to the protected data
    ///
    constexpr reference as_ref() { return _ref; }
    ///
    /// @brief returns the const reference to the protected data
    ///
    /// @return const reference to the protected data
    ///
    constexpr const_reference as_cref() const { return _ref; }
    ///
    /// @brief returns the reference to the protected data
    ///
    /// @return reference to the protected data
    ///
    constexpr reference operator*() { return _ref; }
    ///
    /// @brief returns the const reference to the protected data
    ///
    /// @return const reference to the protected data
    ///
    constexpr const_reference operator*() const { return _ref; }

    ///
    /// @brief returns the reference to the underlying unique_lock
    ///
    /// @return reference to the underlying unique_lock
    ///
    constexpr std::unique_lock<MutexT>& ref_lock() { return _lock; }
    ///
    /// @brief returns the const reference to the underlying unique_lock
    ///
    /// @return const reference to the underlying unique_lock
    ///
    constexpr const std::unique_lock<MutexT>& ref_lock() const { return _lock; }

   private:
    reference _ref;
    std::unique_lock<MutexT> _lock;
};

///
/// @brief Mutex - Mutex protected data
/// @details
/// struct Mutex - provides mutex protection for the data of type T.
///
/// @tparam T data type
/// @tparam MutexT mutex type (default: std::mutex)
///
template <typename T, typename MutexT = std::mutex>
struct Mutex : private ict::core::INotCopyable {
    Mutex() = default;
    constexpr Mutex(const T& value) : _value(value) {}
    constexpr Mutex(T&& value) : _value(std::move(value)) {}

    virtual ~Mutex() = default;

    Mutex(Mutex&&) = default;
    Mutex& operator=(Mutex&&) = default;

    ///
    /// @brief get mutex guard by locking the mutex
    ///
    /// @returns MutexGuard<T, MutexT> mutex guard object
    ///
    constexpr MutexGuard<T, MutexT> lock() { return MutexGuard<T, MutexT>(_value, std::unique_lock<MutexT>(_mutex)); }

    ///
    /// @brief try to get mutex guard by attempting to lock the mutex
    ///
    /// @returns expected<MutexGuard<T, MutexT>, std::error_code> success: mutex guard object, failure: error code
    ///
    constexpr ict::core::expected<MutexGuard<T, MutexT>, std::error_code> try_lock() {
        std::unique_lock<MutexT> ulock(_mutex, std::try_to_lock);
        if (ulock.owns_lock()) {
            return MutexGuard<T, MutexT>(_value, std::move(ulock));
        }
        return ict::core::unexpected(std::make_error_code(std::errc::resource_deadlock_would_occur));
    }

    ///
    /// @brief try to get mutex guard by attempting to lock the mutex with timeout
    ///
    /// @param[in] timeout_duration time duration to wait for locking the mutex
    ///
    /// @returns expected<MutexGuard<T, MutexT>, std::error_code>
    /// success: mutex guard object
    /// timeout: std::errc::timed_out
    /// failure: other error code
    ///
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

