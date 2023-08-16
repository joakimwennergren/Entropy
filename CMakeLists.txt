cmake_minimum_required(VERSION 3.0)

project(
    Symbios
    VERSION 0.0.1
    LANGUAGES CXX
)

# Windowing on desktop
add_subdirectory(external/glfw)

# Logging
add_subdirectory(external/plog)

add_executable(
    # Target
    Symbios
    
    # Source Files 
    "${SOURCE_FILES}"
)

target_sources(Symbios PRIVATE
    # Main entrypoint
    src/main.cpp

    # Core
    src/core/application/application.cpp
    src/core/context/context.cpp
)

target_include_directories(Symbios PRIVATE

    # Config
    src

    # Core
    src/core/application
    src/core/context

    # Metal-CPP
    #external/metal-cpp/
)

target_compile_features(Symbios PRIVATE cxx_std_17)

#target_link_libraries(Symbios "-framework Metal")

target_link_libraries( Symbios
    glfw
    plog
)

if(XCODE)
    set_target_properties(Symbios PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
endif()
