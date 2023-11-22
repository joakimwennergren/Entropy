# _____       _                         
# | ____|_ __ | |_ _ __ ___  _ __  _   _ 
# |  _| | '_ \| __| '__/ _ \| '_ \| | | |
# | |___| | | | |_| | | (_) | |_) | |_| |
# |_____|_| |_|\__|_|  \___/| .__/ \__, |
#                           |_|    |___/ 
# Game Engine version 1.0

# Set minimum cmake version
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# Declare project
project(Entropy)

# Detect platform
include(cmake/platform.cmake)

# Include dependencies
include(cmake/external_dependencies.cmake)

# List source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create the library
add_library(Entropy STATIC ${SOURCES})

# Compiler flags
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wall -Wfloat-conversion)

# Include headers
target_include_directories(${PROJECT_NAME} 
    PRIVATE
        external 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
)

#
target_link_libraries(${PROJECT_NAME}
    sol2
    plog
    glm
    asio::asio
    #libcurl_static 
    libcurl_shared
    freetype
)

#
if(NOT XCODE)
    target_link_libraries(${PROJECT_NAME}
        ${Vulkan_LIBRARIES}
        glfw
    )
endif()

#
configure_file(config.h.in ${CMAKE_SOURCE_DIR}/include/config.hpp @ONLY)