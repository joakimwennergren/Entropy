# iOS Platform
if(XCODE)

    set_target_properties(Entropy PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
    
    target_link_libraries(Entropy

        # Text shaping
        freetype

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