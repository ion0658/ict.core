add_library(ict.core)

generate_export_header(ict.core)

target_include_directories(
    ict.core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(
    ict.core
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}"
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ict.core_export.h"
)

target_sources(
    ict.core
    PRIVATE
        src/formatters.cpp
        src/win_error.cpp
        src/net/ip_addr.cpp
        src/net/ip_addr.v4.cpp
        src/net/ip_addr.v6.cpp
        src/net/sockaddr.cpp
)

target_sources(
    ict.core
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
        include/ict/core/bad_expected_access.hpp
        include/ict/core/concepts.hpp
        include/ict/core/expected.hpp
        include/ict/core/expected.void.hpp
        include/ict/core/formatters.hpp
        include/ict/core/print.hpp
        include/ict/core/unexpected.hpp
        include/ict/core/win_error.hpp
        include/ict/core/net/ip_addr.hpp
        include/ict/core/net/ip_addr.v4.hpp
        include/ict/core/net/ip_addr.v6.hpp
        include/ict/core/net/sockaddr.hpp
        include/ict/core/trait/display.hpp
        include/ict/core/trait/not_copyable.hpp
        include/ict/core/sync/mutex.hpp
        include/ict/core/sync/mutex.void.hpp
        include/ict/core/sync/rwlock.hpp
        include/ict/core/sync/condvar.hpp
        include/ict/core/io/read.hpp
        include/ict/core/io/seek.hpp
        include/ict/core/io/write.hpp
)

if(WIN32)
    target_compile_definitions(
        ict.core
        PRIVATE
            WIN32_LEAN_AND_MEAN
    )
    target_link_libraries(
        ict.core
        PRIVATE
            ws2_32
    )
endif()

install(
    TARGETS ict.core
    EXPORT ict.core
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    ARCHIVE DESTINATION lib/$<CONFIG>
    LIBRARY DESTINATION lib/$<CONFIG>
    RUNTIME DESTINATION bin/$<CONFIG>
)

install(
    EXPORT ict.core
    FILE "ict.core-config.cmake"
    DESTINATION "lib/cmake"
    EXPORT_LINK_INTERFACE_LIBRARIES
)

