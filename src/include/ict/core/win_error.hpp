#pragma once

#ifndef ICT_CORE_WIN_ERROR_HPP
#define ICT_CORE_WIN_ERROR_HPP

#if defined(_MSC_VER)

#include <windows.h>
#include <system_error>

#include "ict.core_export.h"

namespace ict::core {

///
/// @brief Converts a HRESULT to a std::error_code.
///
/// @param[in] hresult Windows HRESULT code.
/// @return std::error_code corresponding to the HRESULT.
///
ICT_CORE_EXPORT std::error_code get_last_error_from_hresult(HRESULT hresult);

///
/// @brief gets the last error from the Windows API and converts it to a std::error_code.
///
/// @return std::error_code corresponding to the last Windows error.
///
ICT_CORE_EXPORT std::error_code get_last_error_from_win32_err();

}  // namespace ict::core

#endif

#endif  // ICT_CORE_WIN_ERROR_HPP

