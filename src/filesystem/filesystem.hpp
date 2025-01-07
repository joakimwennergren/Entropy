#pragma once

#include <config.hpp>
#include <filesystem>
#include <string>

#ifdef BUILD_FOR_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace Entropy::Filesystem
{
  static std::string GetProjectBasePath()
  {
#ifdef BUILD_FOR_WINDOWS
    return ENGINE_BASEPATH;
#endif

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
    return ENGINE_BASEPATH;
#endif

#ifdef BUILD_FOR_IOS
    CFURLRef resourceURL =
        CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
    char resourcePath[PATH_MAX];
    if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8 *)resourcePath,
                                         PATH_MAX))
    {
      if (resourceURL != NULL)
      {
        CFRelease(resourceURL);
      }
      return resourcePath;
    }
#endif

    return "";
  }

  static std::string GetTexturesDir()
  {
#ifdef BUILD_FOR_IOS
    return GetProjectBasePath() + "/";
#else
    return GetProjectBasePath() + "/resources/textures/";
#endif
  }

  static std::string GetScriptsDir()
  {
#ifdef BUILD_FOR_IOS
    return GetProjectBasePath() + "/";
#else
    return GetProjectBasePath() + "/resources/scripts/";
#endif
  }

  static std::string GetSpritesDir()
  {
#ifdef BUILD_FOR_IOS
    return GetProjectBasePath() + "/";
#else
    return GetProjectBasePath() + "/resources/sprites/";
#endif
  }

  static std::string GetShadersDir()
  {
#ifdef BUILD_FOR_IOS
    return GetProjectBasePath() + "/";
#else
    return GetProjectBasePath() + "/shaders/compiled/";
#endif
  }

  static std::string GetFontsDir()
  {
#ifdef BUILD_FOR_IOS
    return GetProjectBasePath() + "/";
#else
    return GetProjectBasePath() + "/resources/fonts/";
#endif
  }
} // namespace Entropy::Filesystem
