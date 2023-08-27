cmake_minimum_required(VERSION 3.7)

project(
    Symbios
    VERSION 0.0.1
    LANGUAGES CXX
)

if(NOT XCODE)
    # Vulkan
    find_package(Vulkan REQUIRED)

    # Windowing on desktop
    add_subdirectory(external/glfw)
endif()

# Logging
add_subdirectory(external/plog)

# GLM
add_subdirectory(external/glm)

# Text shaping
add_subdirectory(external/harfbuzz)

add_executable(Symbios)

target_sources(Symbios PRIVATE

    # Core
    src/core/context/context.cpp

    # Graphics
    src/graphics/pipelines/pipeline.cpp
    src/graphics/shaders/shader.cpp
    src/graphics/renderpasses/renderpass.cpp
    src/graphics/commandbuffers/commandbuffer.cpp
    src/graphics/renderers/renderer.cpp
    src/graphics/buffers/buffer.cpp
    src/graphics/buffers/vertexbuffer.cpp
    src/graphics/textures/texture.cpp
    src/graphics/utilities/utilities.cpp
    src/graphics/primitives/2d/quad.cpp

    # Main entrypoint
    src/main.cpp
)

if(NOT XCODE)
    target_include_directories(Symbios PRIVATE

        # Vulkan
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

target_include_directories(Symbios PRIVATE

    src
    external

    # Core
    src/core/application
    src/core/context

    # Graphics
    src/graphics/pipelines
    src/graphics/shaders
    src/graphics/renderpasses
    src/graphics/commandbuffers
    src/graphics/renderers
    src/graphics/data
    src/graphics/buffers
    src/graphics/textures
    src/graphics/utilities
    src/graphics/primitives/2d
    src/graphics/primitives/3d

    # Audio
    src/audio/

    # Filesystem
    src/filesystem

    # Metal-CPP
    external/metal-cpp/
)

target_compile_features(Symbios PRIVATE cxx_std_17)

target_link_libraries(Symbios

    # Logging
    plog

    # GLM
    glm

    # Text shaping
    harfbuzz
)

if(NOT XCODE)
    target_link_libraries(Symbios

        # Graphics API
        ${Vulkan_LIBRARIES}

        # Windowing
        glfw
    )
endif()

# iOS Platform
if(XCODE)
    set(CMAKE_PLATFORM "BUILD_FOR_IOS")
    set_target_properties(Symbios PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
    target_link_libraries(Symbios
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
    )
    include_directories(external/vulkan-headers)
endif()

# Windows platform
if(WIN32)
    set(CMAKE_PLATFORM "BUILD_FOR_WINDOWS")
endif(WIN32)

# MacOS platform
if(APPLE)
    if(NOT XCODE)
        set(CMAKE_PLATFORM "BUILD_FOR_MACOS")
    endif()
endif()

# Linux platform
if(UNIX AND NOT APPLE)
    set(CMAKE_PLATFORM "BUILD_FOR_LINUX")
endif()

configure_file(config.h.in ${CMAKE_SOURCE_DIR}/src/config.hpp @ONLY)