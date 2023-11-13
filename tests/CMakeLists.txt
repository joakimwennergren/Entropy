cmake_minimum_required(VERSION 3.13)

project(
    Entropy-tests
    VERSION 1.0
    LANGUAGES CXX
)

add_subdirectory(../external/Catch2 "Catch2")


add_executable(Entropy-tests
    unit/core/context/context.test.cpp
)

target_link_libraries(Entropy-tests PRIVATE Catch2::Catch2WithMain)