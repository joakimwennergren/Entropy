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

# Networking
add_subdirectory(external/poco)

add_executable(
    # Target
    Symbios
)

target_sources(Symbios PRIVATE

    # Main entrypoint
    src/main.cpp

    # Core
    src/core/application/application.cpp
    src/core/context/context.cpp

    # Graphics
    src/graphics/pipelines/default_pipeline.cpp
    src/graphics/shaders/default_shader.cpp
    src/graphics/renderpasses/renderpass.cpp
    src/graphics/commandbuffers/commandbuffer.cpp
    src/graphics/renderers/default_renderer.cpp
)

if(NOT XCODE)
    target_include_directories(Symbios PRIVATE

        # Vulkan
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

target_include_directories(Symbios PRIVATE

    src

    # Core
    src/core/application
    src/core/context

    # Graphics
    src/graphics/pipelines
    src/graphics/shaders
    src/graphics/renderpasses
    src/graphics/commandbuffers
    src/graphics/renderers

    # Metal-CPP
    external/metal-cpp/
)

target_compile_features(Symbios PRIVATE cxx_std_17)

target_link_libraries(Symbios

    # Logging
    plog

    # Networking
    PocoFoundation
)

if(NOT XCODE)
    target_link_libraries(Symbios

        # Graphics API
        ${Vulkan_LIBRARIES}

        # Windowing
        glfw
    )
endif()

if(XCODE)
    set_target_properties(Symbios PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
endif()

if(WIN32)
    set(CMAKE_PLATFORM "BUILD_FOR_WINDOWS")
endif(WIN32)

if(APPLE)
    if(NOT XCODE)
        set(CMAKE_PLATFORM "BUILD_FOR_MACOS")
    endif()
endif()

configure_file(config.h.in ${CMAKE_SOURCE_DIR}/src/config.hpp @ONLY)
