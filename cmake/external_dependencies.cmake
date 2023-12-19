# External Dependencies

# GLM matrix maths
add_subdirectory(external/glm)

# Events
add_subdirectory(external/eventpp)

# Logging
add_subdirectory(external/spdlog)

# Networking
#add_subdirectory(external/asio.cmake)

# HTTP 
#set(BUILD_CURL_EXE OFF)
#set(CURL_STATICLIB ON)
#set(SSL_ENABLED ON)

#if(WIN32)
#    set(USE_WINDOWS_SSPI ON)
#   set(USE_SCHANNEL ON)
#endif(WIN32)

#add_subdirectory(external/curl)

# Graphics API

# Vulkan
find_package(Vulkan REQUIRED)

# Windowing on desktop
add_subdirectory(external/glfw)

# Lua SOL 2
add_subdirectory(external/sol2)

# 2D Physics
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(external/box2d)

# Text rendering
set(FT_WITH_BZIP2 OFF)
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