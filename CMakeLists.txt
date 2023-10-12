cmake_minimum_required(VERSION 3.7)

#set(CMAKE_OSX_SYSROOT macosx14.0)

project(
    Symbios
    VERSION 1.0
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

# Lua Cmake
set(LUA_BUILD_COMPILER OFF)
add_subdirectory(external/Lua)

# Lua SOL
add_subdirectory(external/sol2)

# Text rendering
set(FT_WITH_BZIP2 OFF)
add_subdirectory(external/freetype-2.10.0)

add_library(Symbios SHARED
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
    src/graphics/buffers/indexbuffer.cpp
    src/graphics/buffers/uniformbuffer.cpp
    src/graphics/buffers/stagedbuffer.cpp
    src/graphics/textures/texture.cpp
    src/graphics/utilities/utilities.cpp
    src/graphics/primitives/2d/quad.cpp
    src/graphics/primitives/2d/sprite.cpp
    
    # Globals
    src/global/assets.cpp
    src/global/vulkancontext.cpp
    src/global/scenegraph.cpp

    # Text
    src/graphics/text/label.cpp
)


if(NOT XCODE)
    target_include_directories(Symbios PRIVATE
        # Vulkan
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

target_include_directories(Symbios PUBLIC

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
    src/global

    # Audio
    src/audio/

    # Filesystem
    src/filesystem

    # Animation
    src/animation/easing

    # SceneGraph
    src/renderables

    # text
    src/graphics/text

    # Metal-CPP
    external/metal-cpp/

    external/freetype-2.10.0/include

    external/ChaiScript/include
)

target_sources(Symbios PUBLIC FILE_SET HEADERS
  FILES src/symbios.hpp
)

target_compile_features(Symbios PRIVATE cxx_std_17)

target_link_libraries(Symbios

    # Logging
    plog

    # GLM
    glm

    # Scripting
    sol2
    lua_static
)

if(NOT XCODE)
    target_link_libraries(Symbios

        # Graphics API
        ${Vulkan_LIBRARIES}

        # Windowing
        glfw

        # Text shaping
        freetype

        # Scripting
        sol2
        lua_static

    )
endif()

# iOS Platform
if(XCODE)
    set(CMAKE_PLATFORM "BUILD_FOR_IOS")
    set_target_properties(Symbios PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
    
    target_link_libraries(Symbios

        # Text shaping
        freetype

        # Scripting
        sol2
        lua_static

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

# install command needs to reference the file set again
install(TARGETS Symbios FILE_SET HEADERS)

configure_file(config.h.in ${CMAKE_SOURCE_DIR}/src/config.hpp @ONLY)