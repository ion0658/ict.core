#pragma once

#ifndef ICT_CORE_SYNC_CONDVAR_HPP
#define ICT_CORE_SYNC_CONDVAR_HPP

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <system_error>
#include <type_traits>

#include "ict/core/expected.hpp"
#include "ict/core/sync/mutex.hpp"
#include "ict/core/trait/not_copyable.hpp"

namespace ict::core::sync {

///
/// @brief CondVar - condition variable wrapper
/// @details A condition variable is a synchronization primitive that
///
struct CondVar : private ict::core::INotCopyable {
    constexpr CondVar() = default;

    virtual ~CondVar() = default;

    ///
    /// @brief notify_one - notify one waiting thread
    ///
    void notify_one() { _condvar.notify_one(); }
    ///
    /// @brief notify_all - notify all waiting threads
    ///
    void notify_all() { _condvar.notify_all(); }

    ///
    /// @brief wait - wait on the condition variable
    /// @detail Releases the lock on the guarded data and waits until notified.
    ///
    /// @tparam T Type of the guarded data
    /// @param[in] guard MutexGuard object
    ///
    template <typename T, typename MutexType = std::mutex>
    void wait(ict::core::sync::MutexGuard<T, MutexType>& guard) {
        _condvar.wait(guard.ref_lock());
    }

    ///
    /// @brief wait_while - wait on the condition variable while predicate is true
    ///
    /// @tparam T Type of the guarded data
    /// @tparam Predicate Predicate function type
    /// @param[in] guard MutexGuard object
    /// @param[in] pred Predicate function
    ///
    template <typename T, typename Predicate, typename MutexType = std::mutex>
        requires(std::is_invocable_r_v<bool, Predicate>)
    void wait_while(ict::core::sync::MutexGuard<T, MutexType>& guard, Predicate&& pred) {
        _condvar.wait(guard.ref_lock(), pred);
    }

    ///
    /// @brief wait_timeout - wait on the condition variable with timeout
    ///
    /// @tparam T Type of the guarded data
    /// @tparam MutexType Type of the mutex. it should be compatible with wait_for
    /// @param[in] guard MutexGuard object
    /// @param[in] timeout timeout duration
    ///
    /// @return ict::core::expected<void, std::error_code> success: returns void, failure: returns std::errc::timed_out
    ///
    template <typename T, typename MutexType = std::mutex, typename Rep = int64_t, typename Period = std::ratio<1>>
    ict::core::expected<void, std::error_code> wait_timeout(ict::core::sync::MutexGuard<T, MutexType>& guard,
                                                            const std::chrono::duration<Rep, Period> timeout) {
        auto wait_result = _condvar.wait_for(guard.ref_lock(), timeout);
        if (wait_result == std::cv_status::timeout) {
            return ict::core::unexpected(std::make_error_code(std::errc::timed_out));
        }
        return {};
    }

    ///
    /// @brief wait_timeout_while - wait on the condition variable with timeout while predicate is true
    ///
    /// @tparam T Type of the guarded data
    /// @tparam Predicate Predicate function type
    /// @tparam MutexType Type of the mutex. it should be compatible with wait_for
    /// @param[in] guard MutexGuard object
    /// @param[in] pred Predicate function
    /// @param[in] timeout timeout duration
    ///
    template <typename T,
              typename Predicate,
              typename MutexType = std::mutex,
              typename Rep = int64_t,
              typename Period = std::ratio<1>>
        requires(std::is_invocable_r_v<bool, Predicate>)
    void wait_timeout_while(ict::core::sync::MutexGuard<T, MutexType>& guard,
                            Predicate&& pred,
                            const std::chrono::duration<Rep, Period> timeout) {
        _condvar.wait_for(guard.ref_lock(), timeout, pred);
    }

   private:
    std::condition_variable _condvar;
};

}  // namespace ict::core::sync

#endif  // ICT_CORE_SYNC_CONDVAR_HPP

