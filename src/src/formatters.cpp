#include <filesystem>
#include <format>
#include <sstream>
#include <system_error>
#include <thread>
#include <variant>
#include <version>

#include "ict/core/formatters.hpp"

std::format_context::iterator std::formatter<std::monostate, char>::format([[maybe_unused]] const std::monostate& val,
                                                                           std::format_context& ctx) const {
    return std::format_to(ctx.out(), "()");
}
std::wformat_context::iterator std::formatter<std::monostate, wchar_t>::format(
    [[maybe_unused]] const std::monostate& val,
    std::wformat_context& ctx) const {
    return std::format_to(ctx.out(), "()");
}

std::format_context::iterator std::formatter<std::filesystem::path, char>::format(const std::filesystem::path& path,
                                                                                  std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", path.string());
}
std::wformat_context::iterator std::formatter<std::filesystem::path, wchar_t>::format(const std::filesystem::path& path,
                                                                                      std::wformat_context& ctx) const {
    return std::format_to(ctx.out(), "{}", path.string());
}

std::format_context::iterator std::formatter<std::error_code, char>::format(const std::error_code& err,
                                                                            std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{} (code {})", err.message(), err.value());
}
std::wformat_context::iterator std::formatter<std::error_code, wchar_t>::format(const std::error_code& err,
                                                                                std::wformat_context& ctx) const {
    return std::format_to(ctx.out(), "{} (code {})", err.message(), err.value());
}

#ifndef __cpp_lib_formatters
std::format_context::iterator std::formatter<std::thread::id, char>::format(const std::thread::id& tid,
                                                                            std::format_context& ctx) const {
    auto stream = std::stringstream{};
    stream << tid;
    return std::format_to(ctx.out(), "{:s}", stream.str());
}
std::wformat_context::iterator std::formatter<std::thread::id, wchar_t>::format(const std::thread::id& tid,
                                                                                std::wformat_context& ctx) const {
    auto stream = std::stringstream{};
    stream << tid;
    return std::format_to(ctx.out(), "{:s}", stream.str());
}
#endif

