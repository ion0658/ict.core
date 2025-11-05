cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0076 NEW)

project(ict.core VERSION 0.0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(configure)
if(BUILD_DOCS)
    include(doxygen)
endif()

add_subdirectory(3rdparty)
add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

