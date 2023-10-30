cmake_minimum_required(VERSION 3.13)

project(
    Entropy-testsuite
    VERSION 1.0
    LANGUAGES CXX
)

add_subdirectory(../external/Catch2 "Catch2")


add_executable(Entropy-testsuite
    unit/core/context/context.test.cpp
)

target_link_libraries(Entropy-testsuite PRIVATE Catch2::Catch2WithMain)