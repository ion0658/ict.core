#pragma once

#ifndef ICT_CORE_IO_WRITE_HPP
#define ICT_CORE_IO_WRITE_HPP

#include <cstdint>
#include <span>
#include <system_error>

#include "ict/core/expected.hpp"

namespace ict::core::io {

struct IWrite {
    virtual ~IWrite() = default;
    virtual ict::core::expected<size_t, std::error_code> write(const std::span<uint8_t> buffer) = 0;
};

}  // namespace ict::core::io

#endif  // ICT_CORE_IO_WRITE_HPP

