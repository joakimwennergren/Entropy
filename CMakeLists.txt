cmake_minimum_required(VERSION 3.0)

project(
    Symbios
    VERSION 0.0.1
    LANGUAGES CXX
)

if(WIN32 OR APPLE OR UNIX AND NOT XCODE)
    # Vulkan
    find_package(Vulkan REQUIRED)

    # Windowing on desktop
    add_subdirectory(external/glfw)

endif()

# Logging
add_subdirectory(external/plog)

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
)

if(WIN32 OR APPLE OR UNIX AND NOT XCODE)
    target_include_directories(Symbios PRIVATE
        # Vulkan
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

target_include_directories(Symbios PRIVATE

    # Config
    src

    if(WIN32 OR APPLE OR UNIX AND NOT XCODE)

    endif()

    # Core
    src/core/application
    src/core/context

    # Metal-CPP
    external/metal-cpp/
)

if(WIN32 OR APPLE OR UNIX AND NOT XCODE)
    target_include_directories(Symbios PRIVATE
        # Vulkan
        ${Vulkan_INCLUDE_DIRS}
    )
endif()

target_compile_features(Symbios PRIVATE cxx_std_17)

target_link_libraries(Symbios
    # Logging
    plog
)

if(WIN32 OR APPLE OR UNIX AND NOT XCODE)
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
