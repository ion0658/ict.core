#include "ict/core/win_error.hpp"

#if defined(_MSC_VER)

#include <windows.h>
#include <system_error>

namespace ict::core {

std::error_code get_last_error_from_hresult(HRESULT hresult) {
    return std::error_code(hresult, std::system_category());
}

std::error_code get_last_error_from_win32_err() {
    return get_last_error_from_hresult(HRESULT_FROM_WIN32(::GetLastError()));
}

}  // namespace ict::core

#endif

