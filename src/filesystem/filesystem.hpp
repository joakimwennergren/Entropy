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
            return std::filesystem::current_path().string() + "\\..";
#endif
#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
            return std::filesystem::current_path()
                       .string() +
                   "/..";
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
    }
}
