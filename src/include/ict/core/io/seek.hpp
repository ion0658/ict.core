#pragma once

#ifndef ICT_CORE_IO_SEEK_HPP
#define ICT_CORE_IO_SEEK_HPP

#include <cstdint>
#include <system_error>

#include "ict/core/expected.hpp"

namespace ict::core::io {

enum class SeekFrom { Current, Start, End };

struct ISeek {
    virtual ~ISeek() = default;
    virtual ict::core::expected<uint64_t, std::error_code> seek(SeekFrom whence, std::int64_t offset) = 0;
};

}  // namespace ict::core::io

#endif  // ICT_CORE_IO_SEEK_HPP

