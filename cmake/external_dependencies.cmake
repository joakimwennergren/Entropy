# External Dependencies

# Logging 
add_subdirectory(external/plog)

# GLM matrix maths
add_subdirectory(external/glm)

# Networking
add_subdirectory(external/asio.cmake)

# HTTP 
set(BUILD_CURL_EXE OFF)
set(CURL_STATICLIB ON)
set(SSL_ENABLED ON)

if(WIN32)
    set(USE_WINDOWS_SSPI ON)
    set(USE_SCHANNEL ON)
endif(WIN32)

add_subdirectory(external/curl)

# Graphics API
if(NOT XCODE)
    # Vulkan
    find_package(Vulkan REQUIRED)

    # Windowing on desktop
    add_subdirectory(external/glfw)
endif()

# Lua SOL 2
add_subdirectory(external/sol2)

# Text rendering
set(FT_WITH_BZIP2 OFF)
add_subdirectory(external/freetype-2.10.0)