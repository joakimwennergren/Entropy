# Windows platform
if(WIN32)
    set(CMAKE_PLATFORM "BUILD_FOR_WINDOWS")
endif(WIN32)

# MacOS platform
if(APPLE)
    if(NOT XCODE)
        set(CMAKE_PLATFORM "BUILD_FOR_MACOS")
    endif()
endif()

# iOS
if(XCODE)
    set(CMAKE_PLATFORM "BUILD_FOR_IOS")
endif()

# Linux platform
if(UNIX AND NOT APPLE)
    set(CMAKE_PLATFORM "BUILD_FOR_LINUX")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(CMAKE_PLATFORM "BUILD_FOR_ANDROID")
endif()

#set(CMAKE_PLATFORM "BUILD_FOR_ANDROID")