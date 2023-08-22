
#ifdef __APPLE__
#define MA_NO_RUNTIME_LINKING
#endif
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <filesystem>
#include <string>

#ifdef BUILD_FOR_IOS
#include <CoreFoundation/CoreFoundation.h>
#endif

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

int main()
{

    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        return result; // Failed to initialize the engine.
    }

    std::string p = get_resources_dir() + "Anticlimax.wav";
    
    ma_engine_play_sound(&engine, p.c_str(), NULL);

    while(true)
    {

    }

    //Application app;
    //app.Run();
    return 0;
}
