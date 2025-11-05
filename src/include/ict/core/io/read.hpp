#pragma once

#ifndef ICT_CORE_IO_READ_HPP
#define ICT_CORE_IO_READ_HPP

#include <cstdint>
#include <span>
#include <system_error>

#include "ict/core/expected.hpp"

///
/// @brief ict::core::io - Input/Output interfaces
///
namespace ict::core::io {

///
/// @brief IRead - Read interface
/// @details
/// struct IRead - provides an interface for reading data from a source.
///
template <typename ErrorType = std::error_code>
struct IRead {
    using Error = ErrorType;
    virtual ~IRead() = default;
    ///
    /// @brief read - reads data into the provided buffer
    ///
    /// @details
    /// This function attempts to read data into the provided buffer.
    /// returns the number of bytes read on success, or an error code on failure.
    ///
    /// @param[out] buffer destination buffer to read data into
    ///
    /// @returns returns the number of bytes read on success, or an error code on failure
    ///
    virtual ict::core::expected<size_t, Error> read(std::span<uint8_t> buffer) = 0;
};

}  // namespace ict::core::io

#endif  // ICT_CORE_IO_READ_HPP

