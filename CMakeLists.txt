# _____       _
# | ____|_ __ | |_ _ __ ___  _ __  _   _
# |  _| | '_ \| __| '__/ _ \| '_ \| | | |
# | |___| | | | |_| | | (_) | |_) | |_| |
# |_____|_| |_|\__|_|  \___/| .__/ \__, |
#                           |_|    |___/
# Game Engine version 1.0

# Set minimum cmake version
cmake_minimum_required(VERSION 3.30.0)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Declare project
project(Entropy)

# Detect platform
include(cmake/platform.cmake)

# Engine base path
set(PROJECT_BASEPATH ${PROJECT_SOURCE_DIR})

# Include dependencies
include(cmake/external_dependencies.cmake)


file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create the library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    add_definitions(-w)
endif(MSVC)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wno-deprecated -Wno-macro-redefined)

# Include headers
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
        external
        external/vulkan-headers
        external/lua-5.4.6/src
        external/metal-cpp
        external/myers-diff
        external/imgui
        external/bullet3/src
)
if(IOS)
    target_link_libraries(${PROJECT_NAME}
    "-framework foundation"
    "-framework metal"
    "-framework metalkit"
    "-framework uikit"
    "-framework iosurface"
    "-framework coregraphics"
    "-framework quartzcore"
    "-framework coreaudio"
    "-framework audiotoolbox"
    "-framework avfoundation"
    "-framework coretext"
    )

    # Link dependencies for desktop (shouldn't link to vulkanlibs on iOS since wer're using moltenvk)
    target_link_libraries(${PROJECT_NAME}
        TracyClient
        GPUOpen::VulkanMemoryAllocator
        flecs::flecs_static
        BulletCollision
        BulletDynamics
        BulletDynamics
        BulletCollision
        BulletInverseDynamics
        BulletSoftBody
        LinearMath
        tinyobjloader
        sol2
        lua
        glm
        freetype
        eventpp
        spdlog
        box2d
        eventpp
        tinygltf
    )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND APPLE)

    target_link_libraries(${PROJECT_NAME}
        ${Vulkan_LIBRARIES}
        glfw
        TracyClient
        GPUOpen::VulkanMemoryAllocator
        flecs::flecs_static
        BulletCollision
        BulletDynamics
        BulletDynamics
        BulletCollision
        BulletInverseDynamics
        BulletSoftBody
        LinearMath
        tinyobjloader
        sol2
        lua
        glm
        freetype
        eventpp
        spdlog
        box2d
        eventpp
        tinygltf
        libcurl_static
    )
endif()

# Link common dependencies
target_link_libraries(${PROJECT_NAME}
    #asio::asio
    #libcurl_static
    #libcurl_shared
    #OpenSSL::SSL
    #OpenSSL::Crypto
    #ktx
)

# Build config file
configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/config.hpp @ONLY)
