# External Dependencies

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(BUILD_TESTING OFF)
add_compile_definitions(HAVE_FCNTL_H=1)

# GLM matrix maths
add_subdirectory(external/glm)

# Events
add_subdirectory(external/eventpp)

# Logging
add_subdirectory(external/spdlog)

# Networking
# add_subdirectory(external/asio.cmake)

# HTTP
set(BUILD_CURL_EXE OFF)
set(CURL_STATICLIB ON)
set(BUILD_STATIC_LIBS ON)
set(SSL_ENABLED ON)
set(CURL_USE_OPENSSL ON)
set(USE_LIBIDN2 OFF)
set(CURL_DISABLE_ALTSVC ON)
add_subdirectory(external/curl)

# Graphics API

# Vulkan
if (CMAKE_PLATFORM MATCHES "BUILD_FOR_MACOS" OR "BUILD_FOR_WINDOWS" OR "BUILD_FOR_MACOS")
    find_package(Vulkan REQUIRED)
endif ()

# set(KTX_FEATURE_TESTS OFF)
# add_subdirectory(external/KTX-Software)

# Windowing on desktop
if (NOT CMAKE_PLATFORM MATCHES "BUILD_FOR_IOS" )
    add_subdirectory(external/glfw)
endif ()

# Lua SOL 2
add_subdirectory(external/sol2)

# 2D Physics
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(external/box2d)

# Text rendering
set(FT_WITH_BZIP2 OFF)
set(FT_REQUIRE_HARFBUZZ OFF)
set(FT_DISABLE_HARFBUZZ ON)
set(FT_DISABLE_BROTLI ON)
set(FT_DISABLE_PNG ON)
set(FT_DISABLE_ZLIB ON)
add_subdirectory(external/freetype-2.13.2)

set(_LUA_SOURCE_DIR external/lua-5.4.6/src)
set(_LUA_LIB_SRC
        "${_LUA_SOURCE_DIR}/lapi.c"
        "${_LUA_SOURCE_DIR}/lcode.c"
        "${_LUA_SOURCE_DIR}/lctype.c"
        "${_LUA_SOURCE_DIR}/ldebug.c"
        "${_LUA_SOURCE_DIR}/ldo.c"
        "${_LUA_SOURCE_DIR}/ldump.c"
        "${_LUA_SOURCE_DIR}/lfunc.c"
        "${_LUA_SOURCE_DIR}/lgc.c"
        "${_LUA_SOURCE_DIR}/llex.c"
        "${_LUA_SOURCE_DIR}/lmem.c"
        "${_LUA_SOURCE_DIR}/lobject.c"
        "${_LUA_SOURCE_DIR}/lopcodes.c"
        "${_LUA_SOURCE_DIR}/lparser.c"
        "${_LUA_SOURCE_DIR}/lstate.c"
        "${_LUA_SOURCE_DIR}/lstring.c"
        "${_LUA_SOURCE_DIR}/ltable.c"
        "${_LUA_SOURCE_DIR}/ltm.c"
        "${_LUA_SOURCE_DIR}/lundump.c"
        "${_LUA_SOURCE_DIR}/lvm.c"
        "${_LUA_SOURCE_DIR}/lzio.c"
        "${_LUA_SOURCE_DIR}/lauxlib.c"
        "${_LUA_SOURCE_DIR}/lbaselib.c"
        "${_LUA_SOURCE_DIR}/lcorolib.c"
        "${_LUA_SOURCE_DIR}/ldblib.c"
        "${_LUA_SOURCE_DIR}/liolib.c"
        "${_LUA_SOURCE_DIR}/lmathlib.c"
        "${_LUA_SOURCE_DIR}/loadlib.c"
        "${_LUA_SOURCE_DIR}/loslib.c"
        "${_LUA_SOURCE_DIR}/lstrlib.c"
        "${_LUA_SOURCE_DIR}/ltablib.c"
        "${_LUA_SOURCE_DIR}/lutf8lib.c"
        "${_LUA_SOURCE_DIR}/linit.c"
)

add_library(lua OBJECT ${_LUA_LIB_SRC})

# add_compile_definitions(lua PRIVATE LUA_USE_IOS)
add_subdirectory(external/myers-diff)

set(TINYGLTF_INSTALL OFF)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
add_subdirectory(external/tinygltf)

if (ANDROID)
    target_compile_definitions(tinygltf PRIVATE TINYGLTF_ANDROID_LOAD_FROM_ASSETS)
endif ()

# set(BUILD_STATIC_LIBS ON)
# add_subdirectory(external/efsw)
#option(TRACY_ENABLE "" ON)
#option(TRACY_ON_DEMAND "" ON)
#add_subdirectory(external/tracy)

add_subdirectory(external/VulkanMemoryAllocator)

set(FLECS_SHARED OFF)
set(FLECS_PIC OFF)
set(FLECS_TESTS OFF)
add_subdirectory(external/flecs)

set(USE_GRAPHICAL_BENCHMARK OFF)
set(ENABLE_VHACD OFF)
set(BUILD_UNIT_TESTS OFF)
set(BUILD_CPU_DEMOS OFF)
set(BUILD_OPENGL3_DEMOS OFF)
set(BUILD_BULLET2_DEMOS OFF)
set(USE_GLUT OFF)
set(BUILD_EGL OFF)
set(BUILD_BULLET3 OFF)
set(BUILD_ENET OFF)
set(BUILD_CLSOCKET OFF)
set(BUILD_EXTRAS OFF)
set(INSTALL_LIBS OFF)
set(BUILD_UNIT_TESTS OFF)
set(INSTALL_CMAKE_FILES OFF)
add_subdirectory(external/bullet3)

add_subdirectory(external/tinyobjloader)
