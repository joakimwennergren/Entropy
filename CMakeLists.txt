# _____       _                         
# | ____|_ __ | |_ _ __ ___  _ __  _   _ 
# |  _| | '_ \| __| '__/ _ \| '_ \| | | |
# | |___| | | | |_| | | (_) | |_) | |_| |
# |_____|_| |_|\__|_|  \___/| .__/ \__, |
#                           |_|    |___/ 
# Game Engine version 1.0

# Set minimum cmake version
cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Declare project
project(Entropy)

# Detect platform
include(cmake/platform.cmake)

# Engine base path
set(PROJECT_BASEPATH ${PROJECT_SOURCE_DIR})

# Include dependencies
include(cmake/external_dependencies.cmake)

set(IMGUI_DIR external/imgui)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)

# Create the library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    add_definitions(-w)
endif(MSVC)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17)

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
)

# Link common dependencies
target_link_libraries(${PROJECT_NAME}
    sol2
    lua
    glm
    #asio::asio
    #libcurl_static
    #libcurl_shared  
    #OpenSSL::SSL
    #OpenSSL::Crypto
    freetype
    eventpp
    spdlog
    #box2d
    eventpp
    tinygltf
    #ktx
)

# Link dependencies for desktop (shouldn't link to vulkanlibs on iOS since wer're using moltenvk)
target_link_libraries(${PROJECT_NAME}
    ${Vulkan_LIBRARIES}
    glfw
    efsw
    TracyClient
    GPUOpen::VulkanMemoryAllocator
    flecs::flecs_static
)



# Build config file
configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/config.hpp @ONLY)
