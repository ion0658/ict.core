#include <filesystem>
#include <format>
#include <optional>
#include <system_error>
#include <variant>
#include <version>

#ifndef __cpp_lib_formatters
#include <sstream>
#include <thread>
#endif

template <typename CharT>
struct std::formatter<std::monostate, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format([[maybe_unused]] const std::monostate& val, auto& ctx) const { return std::format_to(ctx.out(), "()"); }
};

template <typename CharT>
struct std::formatter<std::filesystem::path, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const std::filesystem::path& val, auto& ctx) const {
        return std::format_to(ctx.out(), "{}", val.string());
    }
};

template <typename CharT>
struct std::formatter<std::error_code, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const std::error_code& val, auto& ctx) const {
        return std::format_to(ctx.out(), "{} (code {})", val.message(), val.value());
    }
};

#ifndef __cpp_lib_formatters
template <typename CharT>
struct std::formatter<std::thread::id, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const std::thread::id& val, auto& ctx) const {
        auto stream = std::stringstream{};
        stream << val;
        return std::format_to(ctx.out(), "{:s}", stream.str());
    }
};
#endif

template <typename T, typename CharT>
struct std::formatter<std::optional<T>, CharT> : std::formatter<std::basic_string<CharT>, CharT> {
    auto format(const std::optional<T>& val, auto& ctx) const {
        if (val.has_value()) {
            return std::format_to(ctx.out(), "Some({})", val.value());
        } else {
            return std::format_to(ctx.out(), "None");
        }
    }
};

