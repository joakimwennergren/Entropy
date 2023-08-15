cmake_minimum_required(VERSION 3.0)

project(
    Symbios
    VERSION 0.0.1
    LANGUAGES CXX C # programming languages used by the project
)

add_executable(Symbios)

target_sources(Symbios PRIVATE
    src/main.cpp
)

target_include_directories(Symbios PRIVATE

    # Metal-CPP
    external/metal-cpp/
)

target_compile_features(Symbios PRIVATE cxx_std_17)

target_link_libraries(Symbios "-framework Metal")

if(XCODE)
    set_target_properties(Symbios PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
endif()
