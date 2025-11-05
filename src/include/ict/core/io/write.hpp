#pragma once

#ifndef ICT_CORE_IO_WRITE_HPP
#define ICT_CORE_IO_WRITE_HPP

#include <cstdint>
#include <span>
#include <system_error>

#include "ict/core/expected.hpp"

namespace ict::core::io {

///
/// @brief IWrite - Write interface
/// @details
/// struct IWrite - provides an interface for writing data to a sink.
///
template <typename ErrorType = std::error_code>
struct IWrite {
    using Error = ErrorType;
    virtual ~IWrite() = default;
    ///
    /// @brief writes data from the provided buffer
    ///
    /// @param[in] buffer buffer containing data to write
    ///
    /// @return number of bytes written or an error
    ///
    virtual ict::core::expected<size_t, Error> write(const std::span<uint8_t> buffer) = 0;

    ///
    /// @brief flush - flushes any buffered data to the sink
    ///
    /// @return void or an error
    ///
    virtual ict::core::expected<void, Error> flush() = 0;
};

}  // namespace ict::core::io

#endif  // ICT_CORE_IO_WRITE_HPP

