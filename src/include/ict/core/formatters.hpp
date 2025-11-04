#include <filesystem>
#include <format>
#include <optional>
#include <system_error>
#include <thread>
#include <variant>
#include <version>

#include "ict.core_export.h"

template <>
struct ICT_CORE_EXPORT std::formatter<std::monostate, char> : std::formatter<std::string, char> {
    std::format_context::iterator format(const std::monostate& val, std::format_context& ctx) const;
};

template <>
struct ICT_CORE_EXPORT std::formatter<std::monostate, wchar_t> : std::formatter<std::wstring, wchar_t> {
    std::wformat_context::iterator format(const std::monostate& val, std::wformat_context& ctx) const;
};

template <>
struct ICT_CORE_EXPORT std::formatter<std::filesystem::path, char> : std::formatter<std::string, char> {
    std::format_context::iterator format(const std::filesystem::path& path, std::format_context& ctx) const;
};
template <>
struct ICT_CORE_EXPORT std::formatter<std::filesystem::path, wchar_t> : std::formatter<std::wstring, wchar_t> {
    std::wformat_context::iterator format(const std::filesystem::path& path, std::wformat_context& ctx) const;
};

template <>
struct ICT_CORE_EXPORT std::formatter<std::error_code, char> : std::formatter<std::string, char> {
    std::format_context::iterator format(const std::error_code& err, std::format_context& ctx) const;
};
template <>
struct ICT_CORE_EXPORT std::formatter<std::error_code, wchar_t> : std::formatter<std::wstring, wchar_t> {
    std::wformat_context::iterator format(const std::error_code& err, std::wformat_context& ctx) const;
};

#ifndef __cpp_lib_formatters
template <>
struct ICT_CORE_EXPORT std::formatter<std::thread::id, char> : std::formatter<std::string, char> {
    std::format_context::iterator format(const std::thread::id& tid, std::format_context& ctx) const;
};
template <>
struct ICT_CORE_EXPORT std::formatter<std::thread::id, wchar_t> : std::formatter<std::wstring, wchar_t> {
    std::wformat_context::iterator format(const std::thread::id& tid, std::wformat_context& ctx) const;
};
#endif

template <typename T>
struct std::formatter<std::optional<T>, char> : std::formatter<std::string, char> {
    std::format_context::iterator format(const std::optional<T>& opt, std::format_context& ctx) const {
        if (opt.has_value()) {
            return std::format_to(ctx.out(), "Some({})", opt.value());
        } else {
            return std::format_to(ctx.out(), "None");
        }
    }
};
template <typename T>
struct std::formatter<std::optional<T>, wchar_t> : std::formatter<std::wstring, wchar_t> {
    std::wformat_context::iterator format(const std::optional<T>& opt, std::wformat_context& ctx) const {
        if (opt.has_value()) {
            return std::format_to(ctx.out(), "Some({})", opt.value());
        } else {
            return std::format_to(ctx.out(), "None");
        }
    }
};

