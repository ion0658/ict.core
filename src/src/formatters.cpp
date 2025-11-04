#include "ict/core/formatters.hpp"
#include "ict.core_export.h"

template <>
struct ICT_CORE_EXPORT std::formatter<std::monostate, char>;

template <>
struct ICT_CORE_EXPORT std::formatter<std::filesystem::path, char>;

template <>
struct ICT_CORE_EXPORT std::formatter<std::error_code, char>;

#ifndef __cpp_lib_formatters
template <>
struct ICT_CORE_EXPORT std::formatter<std::thread::id, char>;
#endif

