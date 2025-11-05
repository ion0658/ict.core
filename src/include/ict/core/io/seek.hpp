#pragma once

#ifndef ICT_CORE_IO_SEEK_HPP
#define ICT_CORE_IO_SEEK_HPP

#include <cstdint>
#include <system_error>

#include "ict/core/expected.hpp"

namespace ict::core::io {

///
/// @brief SeekFrom - Seek origin
/// @details
/// enum SeekFrom - origin for seeking
///
enum class SeekFrom {
    /// see from the current position
    Current,
    /// seek from the beginning
    Start,
    /// seek from the end
    End
};

///
/// @brief SeekPosition - Seek position
/// @details
/// struct SeekPosition - represents a position to seek to
///
struct SeekPosition {
    ///
    /// @brief create a SeekPosition from current position
    ///
    /// @param[in] offset offset from current position
    /// @return SeekPosition
    ///
    constexpr static SeekPosition current(int64_t offset) {
        auto pos = SeekPosition{};
        pos._from = SeekFrom::Current;
        pos._offset = offset;
        return pos;
    }

    ///
    /// @brief create a SeekPosition from the beginning
    ///
    /// @param[in] offset offset from the beginning
    /// @return SeekPosition
    ///
    constexpr static SeekPosition start(uint64_t offset) {
        auto pos = SeekPosition{};
        pos._from = SeekFrom::Start;
        pos._offset = offset;
        return pos;
    }
    ///
    /// @brief create a SeekPosition from the end
    ///
    /// @param[in] offset offset from the end
    /// @return SeekPosition
    ///
    constexpr static SeekPosition end(uint64_t offset) {
        auto pos = SeekPosition{};
        pos._from = SeekFrom::End;
        pos._offset = offset;
        return pos;
    }
    ///
    /// @brief get the seek origin
    ///
    /// @return SeekFrom
    ///
    constexpr SeekFrom from() const { return _from; }
    ///
    /// @brief get the offset as signed integer
    /// @warning Only valid when SeekFrom is Current
    ///
    /// @return offset as signed integer
    ///
    constexpr int64_t get_offset_as_signed() const { return std::get<int64_t>(_offset); }
    ///
    /// @brief get the offset as unsigned integer
    /// @warning Only valid when SeekFrom is Start or End
    ///
    /// @return offset as unsigned integer
    ///
    constexpr uint64_t get_offset_as_unsigned() const { return std::get<uint64_t>(_offset); }

   private:
    SeekPosition() = default;
    SeekFrom _from;
    std::variant<int64_t, uint64_t> _offset;
};

///
/// @brief ISeek - Seek interface
/// @details
/// struct ISeek - provides an interface for seeking to a position in a data source.
///
template <typename ErrorType = std::error_code>
struct ISeek {
    using Error = ErrorType;
    virtual ~ISeek() = default;
    ///
    /// @brief seek to the specified position
    ///
    /// @param[in] pos position to seek to
    ///
    /// @return expected<uint64_t, Error> new position after seek
    ///
    virtual ict::core::expected<uint64_t, Error> seek(SeekPosition pos) = 0;
};

}  // namespace ict::core::io

#endif  // ICT_CORE_IO_SEEK_HPP

