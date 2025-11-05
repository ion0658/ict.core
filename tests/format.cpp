#include <filesystem>
#include <format>
#include <optional>
#include <system_error>
#include <thread>
#include <variant>

#include "gtest/gtest.h"

#include "ict/core/formatters.hpp"

TEST(formatters, monostate) {
    auto val = std::monostate{};
    EXPECT_EQ(std::format("{}", val), "()");
}

TEST(formatters, filesystem_path) {
    auto val = std::filesystem::path{"/some/path/to/file.txt"};
    EXPECT_EQ(std::format("{}", val), "/some/path/to/file.txt");
}

TEST(formatters, error_code) {
    auto val = std::make_error_code(std::errc::no_such_file_or_directory);
    EXPECT_EQ(std::format("{}", val), std::format("{} (code {})", val.message(), val.value()));
}

TEST(formatters, thread_id) {
    auto val = std::this_thread::get_id();
    auto formatted = std::format("{}", val);
    auto stream = std::stringstream{};
    stream << val;
    EXPECT_EQ(formatted, stream.str());
}

TEST(formatters, optional) {
    auto val_some = std::optional<int>{42};
    auto val_none = std::optional<int>{};
    EXPECT_EQ(std::format("{}", val_some), "Some(42)");
    EXPECT_EQ(std::format("{}", val_none), "None");
}

