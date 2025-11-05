#pragma once

#ifndef ICT_CORE_PRINT_HPP
#define ICT_CORE_PRINT_HPP

#include <version>

#ifdef __cpp_lib_print

#include <ostream>
#include <print>

namespace ict::core {
// use std::print if available
using print = std::print;
}  // namespace ict::core

#else

#include <cstdio>
#include <format>
#include <ostream>

namespace ict::core {

///
/// @brief print formatted output to a file stream
///
/// @tparam Args Format argument types
/// @param[in] stream output file stream
/// @param[in] fmt format string
/// @param[in] args format arguments
///
template <class... Args>
void print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args) {
    std::string output = std::format(fmt, std::forward<Args>(args)...);
    std::fwrite(output.data(), sizeof(char), output.size(), stream);
}

///
/// @brief print formatted output to an output stream
///
/// @tparam Args Format argument types
/// @param[in] ostream output stream
/// @param[in] fmt format string
/// @param[in] args format arguments
///
template <class... Args>
void print(std::ostream& ostream, std::format_string<Args...> fmt, Args&&... args) {
    ostream << std::format(fmt, std::forward<Args>(args)...);
}

///
/// @brief print formatted output to standard output
///
/// @tparam Args Format argument types
/// @param[in] fmt format string
/// @param[in] args format arguments
///
template <class... Args>
void print(std::format_string<Args...> fmt, Args&&... args) {
    print(stdout, fmt, std::forward<Args>(args)...);
}

///
/// @brief print formatted output with a newline to a file stream
///
/// @tparam Args Format argument types
/// @param[in] stream output file stream
/// @param[in] fmt format string
/// @param[in] args format arguments
///
template <class... Args>
void println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args) {
    print(stream, "{}\n", std::format(fmt, std::forward<Args>(args)...));
}

///
/// @brief print a newline to a file stream
///
/// @param[in] stream output file stream
///
void println(std::FILE* stream) {
    print(stream, "\n");
}

///
/// @brief print formatted output with a newline to output stream
///
/// @tparam Args Format argument types
/// @param[in] ostream output stream
/// @param[in] fmt format string
/// @param[in] args format arguments
///
template <class... Args>
void println(std::ostream& ostream, std::format_string<Args...> fmt, Args&&... args) {
    print(ostream, "{}\n", std::format(fmt, std::forward<Args>(args)...));
}

///
/// @brief print a newline to an output stream
///
/// @param[in] ostream output stream
///
void println(std::ostream& ostream) {
    print(ostream, "\n");
}

///
/// @brief print formatted output with a newline to standard output
///
/// @tparam Args Format argument types
/// @param[in] fmt format string
/// @param[in] args format arguments
///
template <class... Args>
void println(std::format_string<Args...> fmt, Args&&... args) {
    println(stdout, fmt, std::forward<Args>(args)...);
}

///
/// @brief print a newline to standard output
///
void println() {
    println(stdout);
}

}  // namespace ict::core

#endif
#endif  // ICT_CORE_PRINT_HPP

