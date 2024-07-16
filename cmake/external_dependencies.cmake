# External Dependencies

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_compile_definitions(HAVE_FCNTL_H=1)

# GLM matrix maths
add_subdirectory(external/glm)

# Events
add_subdirectory(external/eventpp)

# Logging
add_subdirectory(external/spdlog)

# Networking
#add_subdirectory(external/asio.cmake)

# HTTP 
set(BUILD_CURL_EXE OFF)
set(CURL_STATICLIB ON)
set(SSL_ENABLED ON)

if(WIN32)
    set(USE_WINDOWS_SSPI ON)
    set(USE_SCHANNEL ON)
else()
  set(BUILD_OPENSSL ON)
  set(OPENSSL_BUILD_VERSION 3.1.5)
  #add_subdirectory(external/openssl-cmake ${CMAKE_CURRENT_BINARY_DIR}/external/openssl-cmake)
  add_library(OpenSSL::SSL OBJECT IMPORTED)
  add_library(OpenSSL::Crypto OBJECT IMPORTED)
  set_target_properties(OpenSSL::SSL PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/external/openssl-cmake/usr/local/lib/libssl.a)
  set_target_properties(OpenSSL::Crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/external/openssl-cmake/usr/local/lib/libcrypto.a)
  set(OPENSSL_ROOT_DIR ${CMAKE_BINARY_DIR}/external/openssl-cmake/usr/local)
  set(OPENSSL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/openssl-cmake/usr/local/include)

  set(HAVE_OPENSSL_CRYPTO_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_OPENSSL_ERR_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_OPENSSL_PEM_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_OPENSSL_RSA_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_OPENSSL_SSL_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_OPENSSL_X509_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_OPENSSL_RAND_H TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_RAND_STATUS TRUE CACHE INTERNAL "" FORCE)
  set(HAVE_RAND_SCREEN FALSE CACHE INTERNAL "" FORCE)
  set(HAVE_RAND_EGD FALSE CACHE INTERNAL "" FORCE)
endif(WIN32)


set(CURL_USE_OPENSSL OFF)
#add_subdirectory(external/curl)

# Graphics API

# Vulkan
find_package(Vulkan REQUIRED)

#set(KTX_FEATURE_TESTS OFF)
#add_subdirectory(external/KTX-Software)

# Windowing on desktop
if(WIN32 OR UNIX OR APPLE)
  add_subdirectory(external/glfw)
endif()

# Lua SOL 2
add_subdirectory(external/sol2)

# 2D Physics
#set(BOX2D_BUILD_UNIT_TESTS OFF)
#set(BOX2D_BUILD_TESTBED OFF)
#add_subdirectory(external/box2d)

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
#add_compile_definitions(lua PRIVATE LUA_USE_IOS)

add_subdirectory(external/myers-diff)

set(TINYGLTF_INSTALL OFF)
set(TINYGLTF_BUILD_LOADER_EXAMPLE  OFF)
add_subdirectory(external/tinygltf)

if(ANDROID)
  target_compile_definitions(tinygltf PRIVATE TINYGLTF_ANDROID_LOAD_FROM_ASSETS)
endif()

#set(BUILD_STATIC_LIBS ON)
#add_subdirectory(external/efsw)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(external/tracy)


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

set(KANGARU_INSTALL FALSE)
set(KANGARU_EXPORT FALSE)
set(KANGARU_REVERSE_DESTRUCTION TRUE)
add_subdirectory(external/kangaru)
