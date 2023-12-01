#include "shader.h"

uint32_t * ReadShader(android_app *app, std::string filePath)
{
    AAsset* file = AAssetManager_open(app->activity->assetManager,
                                      filePath, AASSET_MODE_BUFFER);

    size_t fileLength = AAsset_getLength(file);
    char* fileContent = new char[fileLength];
    AAsset_read(file, fileContent, fileLength);
    AAsset_close(file);
    return static_cast<uint32_t *>(fileContent);
}