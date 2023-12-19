# _____       _                         
# | ____|_ __ | |_ _ __ ___  _ __  _   _ 
# |  _| | '_ \| __| '__/ _ \| '_ \| | | |
# | |___| | | | |_| | | (_) | |_) | |_| |
# |_____|_| |_|\__|_|  \___/| .__/ \__, |
#                           |_|    |___/ 
# Game Engine version 1.0

# Set minimum cmake version
cmake_minimum_required(VERSION 3.6)

# Declare project
project(Entropy)

# Detect platform
include(cmake/platform.cmake)

# Engine base path
set(PROJECT_BASEPATH ${PROJECT_SOURCE_DIR})

# Include dependencies
include(cmake/external_dependencies.cmake)

# List source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp )

# Create the library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Compiler flags
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wall -Wfloat-conversion)

# Include headers
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
        external 
        external/vulkan-headers
        external/lua-5.4.6/src
)

# Link common dependencies
target_link_libraries(${PROJECT_NAME}
    sol2
    lua
    glm
    #asio::asio
    #libcurl_shared #libcurl_static 
    freetype
    eventpp
    spdlog
    box2d
    eventpp
)

# Link dependencies for desktop (shouldn't link to vulkanlibs on iOS since wer're using moltenvk)
target_link_libraries(${PROJECT_NAME}
    ${Vulkan_LIBRARIES}
    glfw
)

# Build config file
configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/config.hpp @ONLY)
