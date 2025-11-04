add_library(ict.core)

generate_export_header(ict.core)

target_sources(
    ict.core
    PRIVATE
        src/formatters.cpp
        src/net/ip_addr.cpp
        src/net/ip_addr.v4.cpp
        src/net/ip_addr.v6.cpp
)

target_include_directories(
    ict.core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(
    ict.core
    PROPERTIES
    PUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ict.core_export.h;include/expected.hpp;"
)

if(WIN32)
    target_compile_definitions(
        ict.core
        PRIVATE
            WIN32_LEAN_AND_MEAN
    )
endif()

install(
    TARGETS ict.core
    EXPORT ict.core
    PUBLIC_HEADER DESTINATION include/ict/core
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

