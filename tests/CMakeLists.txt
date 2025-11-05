add_executable(ict.core_test)

target_link_libraries(
    ict.core_test
    PRIVATE
        ict.core
        gtest_main
)

target_sources(
    ict.core_test
    PRIVATE
        expected.cpp
)

add_test(
    NAME ict.core_test
    COMMAND ict.core_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "21")
    message(WARNING "Googletest has a bug recognized by Clang 21+. "
     "Supressing character-conversion warning. "
     "Consider using an older version of Clang.")
    target_compile_options(gtest_main PRIVATE -Wno-error=character-conversion)
    target_compile_options(gtest PRIVATE -Wno-error=character-conversion)
endif()

