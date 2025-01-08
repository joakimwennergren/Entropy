if(CMAKE_PLATFORM MATCHES "BUILD_FOR_IOS")
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
        libcurl_static
    )
endif()

if(CMAKE_PLATFORM MATCHES "BUILD_FOR_MACOS")
    target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wno-deprecated -Wno-macro-redefined)
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

if(CMAKE_PLATFORM MATCHES "BUILD_FOR_WINDOWS")
    target_link_libraries(${PROJECT_NAME}
        ${Vulkan_LIBRARIES}
        glfw

        # TracyClient
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

        # libcurl_static
    )
endif(WIN32)