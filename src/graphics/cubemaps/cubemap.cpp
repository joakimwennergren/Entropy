#include "cubemap.hpp"

using namespace Entropy::Graphics::CubeMaps;

CubeMap::CubeMap(std::shared_ptr<ServiceLocator> serviceLocator)
{
    _serviceLocator = serviceLocator;
    type = 10;
    _texture = std::make_shared<Texture>(_serviceLocator);
    _model = std::make_unique<Model>(_serviceLocator);
}

// Loads a cubemap from a file, uploads it to the device and create all Vulkan resources required to display it
void CubeMap::LoadCubemap(std::string filename, VkFormat format)
{

    auto logicalDevice = _serviceLocator->GetService<LogicalDevice>();

    ktxResult result;
    ktxTexture *ktxTexture;

#if defined(__ANDROID__)
    /*
    // Textures are stored inside the apk on Android (compressed)
    // So they need to be loaded via the asset manager
    AAsset *asset = AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
    if (!asset)
    {
        vks::tools::exitFatal("Could not load texture from " + filename + "\n\nMake sure the assets submodule has been checked out and is up-to-date.", -1);
    }
    size_t size = AAsset_getLength(asset);
    assert(size > 0);

    ktx_uint8_t *textureData = new ktx_uint8_t[size];
    AAsset_read(asset, textureData, size);
    AAsset_close(asset);
    result = ktxTexture_CreateFromMemory(textureData, size, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
    delete[] textureData;
     */
#else
    result = ktxTexture_CreateFromNamedFile(filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
#endif
    assert(result == KTX_SUCCESS);

    // Get properties required for using and upload texture data from the ktx texture object
    uint32_t width = ktxTexture->baseWidth;
    uint32_t height = ktxTexture->baseHeight;
    uint32_t mipLevels = ktxTexture->numLevels;
    ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
    ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(ktxTexture);

    _texture->CreateTextureImageFromKtx(ktxTextureData, width, height, ktxTextureSize, mipLevels, format, ktxTexture);
}
