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

struct CondVar : private ict::core::INotCopyable {
    constexpr CondVar() = default;

    virtual ~CondVar() = default;

    void notify_one() { _condvar.notify_one(); }
    void notify_all() { _condvar.notify_all(); }

    template <typename T, typename MutexType = std::mutex>
    void wait(ict::core::sync::MutexGuard<T, MutexType>& guard) {
        _condvar.wait(guard.ref_lock());
    }

    template <typename T, typename Predicate, typename MutexType = std::mutex>
        requires(std::is_invocable_r_v<bool, Predicate>)
    void wait_while(ict::core::sync::MutexGuard<T, MutexType>& guard, Predicate&& pred) {
        _condvar.wait(guard.ref_lock(), pred);
    }

    template <typename T, typename MutexType = std::mutex, typename Rep = int64_t, typename Period = std::ratio<1>>
    ict::core::expected<void, std::error_code> wait_timeout(ict::core::sync::MutexGuard<T, MutexType>& guard,
                                                            const std::chrono::duration<Rep, Period> timeout) {
        auto wait_result = _condvar.wait_for(guard.ref_lock(), timeout);
        if (wait_result == std::cv_status::timeout) {
            return ict::core::unexpected(std::make_error_code(std::errc::timed_out));
        }
        return {};
    }

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

