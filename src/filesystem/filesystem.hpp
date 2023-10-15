#pragma once

#include <filesystem>
#include <string>

#ifdef BUILD_FOR_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace Symbios
{
    namespace Filesystem
    {
        inline static std::string GetProjectBasePath()
        {

#ifdef BUILD_FOR_WINDOWS
            return "../../";
#endif

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
            return std::filesystem::current_path().string() + "/..";
#endif

#ifdef BUILD_FOR_IOS
            CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
            char resourcePath[PATH_MAX];
            if (CFURLGetFileSystemRepresentation(resourceURL, true,
                                                 (UInt8 *)resourcePath,
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

        inline static std::string GetTexturesDir()
        {
            #ifdef BUILD_FOR_IOS
                return Filesystem::GetProjectBasePath() + "/";
            #else 
                return Filesystem::GetProjectBasePath() + "/resources/textures/"; 
            #endif
        }

        inline static std::string GetScriptsDir()
        {
            #ifdef BUILD_FOR_IOS
                return Filesystem::GetProjectBasePath() + "/";
            #else 
                return Filesystem::GetProjectBasePath() + "/resources/scripts/"; 
            #endif
        }

        inline static std::string GetSpritesDir()
        {
            #ifdef BUILD_FOR_IOS
                return Filesystem::GetProjectBasePath() + "/";
            #else 
                return Filesystem::GetProjectBasePath() + "/resources/sprites/"; 
            #endif
        }

        inline static std::string GetShadersDir()
        {
            #ifdef BUILD_FOR_IOS
                return Filesystem::GetProjectBasePath() + "/";
            #else 
                return Filesystem::GetProjectBasePath() + "/shaders/basic/"; 
            #endif
        }

        inline static std::string GetFontsDir()
        {
            #ifdef BUILD_FOR_IOS
                return Filesystem::GetProjectBasePath() + "/";
            #else 
                return Filesystem::GetProjectBasePath() + "/resources/fonts/"; 
            #endif
        }
    }
}
