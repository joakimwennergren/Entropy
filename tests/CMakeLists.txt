cmake_minimum_required(VERSION 3.6)

project(
    Entropy-tests
    VERSION 1.0
    LANGUAGES CXX
)
add_subdirectory(../ Entropy)
add_subdirectory(../external/Catch2 "Catch2")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/unit/*.cpp ${PROJECT_SOURCE_DIR}/unit/utils/setup.cpp)

include_directories(${PROJECT_SOURCE_DIR}/unit/utils)

add_executable(Entropy-tests ${SOURCES})

target_link_libraries(Entropy-tests PRIVATE Catch2::Catch2WithMain Entropy)