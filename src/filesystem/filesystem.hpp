#pragma once

#include <filesystem>

#ifdef BUILD_FOR_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace Symbios
{
    namespace Filesystem
    {
        static std::string GetProjectBasePath()
        {
            return std::filesystem::current_path().string() + "../";
        }

#ifdef BUILD_FOR_MACOS
        class MacOSFilesystem
        {
        };
#endif
#ifdef BUILD_FOR_IOS
        class IOSFilesystem
        {
        public:
            IOSFilesystem();

            std::string get_resources_dir()
            {
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
                return "";
            }

        private:
        };
#endif
    }
}