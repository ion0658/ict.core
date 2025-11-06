set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

include(GenerateExportHeader)
include(FetchContent)

find_program(SCCACHE_EXE sccache)
if(SCCACHE_EXE)
    message(STATUS "sccache found: ${SCCACHE_EXE}")
    set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_EXE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_EXE})
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT_DEBUG Embedded)
    cmake_policy(SET CMP0141 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4
        /MP
        /utf-8
        /WX
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Werror
    )
endif()

