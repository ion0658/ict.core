#pragma once

#ifndef ICT_CORE_IO_READ_HPP
#define ICT_CORE_IO_READ_HPP

#include <cstdint>
#include <span>
#include <system_error>

#include "ict/core/expected.hpp"

namespace ict::core::io {

struct IRead {
    virtual ~IRead() = default;
    virtual ict::core::expected<size_t, std::error_code> read(std::span<uint8_t> buffer) = 0;
};

}  // namespace ict::core::io

#endif  // ICT_CORE_IO_READ_HPP

