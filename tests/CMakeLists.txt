cmake_minimum_required(VERSION 3.13)

set (WILDCARD_SOURCE *.cpp)

file(GLOB_RECURSE TEST_SOURCES ${WILDCARD_SOURCE})

add_executable(${TARGET_TEST} ${TEST_SOURCES})

# Testing
add_subdirectory(external/Catch2)

# Link to the desired libraries
target_link_libraries(${TARGET_TEST}
    PRIVATE
    Catch2::Catch2
)

target_compile_definitions(DcmlParserTest PRIVATE
    TEST_DATA_PATH="${PROJECT_SOURCE_DIR}/data")