#pragma once

#ifndef ICT_CORE_WIN_ERROR_HPP
#define ICT_CORE_WIN_ERROR_HPP

#if defined(_MSC_VER)

#include <windows.h>
#include <system_error>

#include "ict.core_export.h"

namespace ict::core {

ICT_CORE_EXPORT std::error_code get_last_error_from_hresult(HRESULT hresult);
ICT_CORE_EXPORT std::error_code get_last_error_from_win32_err();

}  // namespace ict::core

#endif

#endif  // ICT_CORE_WIN_ERROR_HPP

