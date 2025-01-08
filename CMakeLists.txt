# ______ _   _ _______ _____   ____  _______     __
# |  ____| \ | |__   __|  __ \ / __ \|  __ \ \   / /
# | |__  |  \| |  | |  | |__) | |  | | |__) \ \_/ /
# |  __| | . ` |  | |  |  _  /| |  | |  ___/ \   /
# | |____| |\  |  | |  | | \ \| |__| | |      | |
# |______|_| \_|  |_|  |_|  \_\\____/|_|      |_|  v 1.0

# Set minimum cmake version
cmake_minimum_required(VERSION 3.28.1)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Declare project
project(Entropy)

# Detect platform
include(cmake/platform.cmake)

# Engine base path
set(PROJECT_BASEPATH ${PROJECT_SOURCE_DIR})

# Include dependencies
include(cmake/external_dependencies.cmake)

# Glob source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create the library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Include headers
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include

        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
        external

        # external/vulkan-headers
        external/lua-5.4.6/src
        external/metal-cpp
        external/myers-diff
        external/imgui
        external/bullet3/src
        external/curl/include
        include
)

# Compiler flags
include(cmake/compiler_flags.cmake)

# Link libraries
include(cmake/linking.cmake)

# Build config file
configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/config.hpp @ONLY)
