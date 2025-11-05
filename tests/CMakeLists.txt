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
        format.cpp
        ip_addr.cpp
        ip_addr.v4.cpp
        ip_addr.v6.cpp
        mutex.cpp
        unexpected.cpp
)

add_test(
    NAME ict.core_test
    COMMAND ict.core_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

