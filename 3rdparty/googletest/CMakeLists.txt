if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(googletest)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "21")
        message(WARNING "Googletest has a bug recognized by Clang 21+. "
     "Supressing character-conversion warning. "
     "Consider using an older version of Clang.")
        target_compile_options(gtest_main PRIVATE -Wno-error=character-conversion)
        target_compile_options(gtest PRIVATE -Wno-error=character-conversion)
    endif()

endif()

