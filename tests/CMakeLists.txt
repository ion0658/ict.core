function(ict_add_test test_name src_file)
    add_executable(ict.core.${test_name}.test)
    target_sources(
        ict.core.${test_name}.test
        PRIVATE
            ${src_file}
    )
    target_link_libraries(
        ict.core.${test_name}.test
        PRIVATE
            ict.core
            gtest_main
    )
    add_test(
        NAME ict.core.${test_name}.test
        COMMAND ict.core.${test_name}.test
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()

ict_add_test(condvar condvar.cpp)
ict_add_test(expected expected.cpp)
ict_add_test(format format.cpp)
ict_add_test(ip_addr ip_addr.cpp)
ict_add_test(ip_addr_v4 ip_addr.v4.cpp)
ict_add_test(ip_addr_v6 ip_addr.v6.cpp)
ict_add_test(mutex mutex.cpp)
ict_add_test(rwlock rwlock.cpp)
ict_add_test(unexpected unexpected.cpp)

