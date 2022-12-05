cmake_minimum_required(VERSION 3.20)
project(formal_lang)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS --coverage)

add_executable(unit_tests tests/tests.cpp src/CYK_algo.cpp
        src/CYK_algo.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()

target_link_libraries(
        unit_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
