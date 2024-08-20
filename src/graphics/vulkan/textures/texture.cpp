#include "texture.hpp"

using namespace Entropy::Graphics::Vulkan::Textures;

// void Texture::CreateTextureFromGLTFImage(tinygltf::Image &gltfimage)
// {

/*

auto physicalDevice = _serviceLocator->GetService<PhysicalDevice>();

unsigned char *buffer = nullptr;
VkDeviceSize bufferSize = 0;
bool deleteBuffer = false;
if (gltfimage.component == 3) {
  // Most devices don't support RGB only on Vulkan so convert if necessary
  // TODO: Check actual format support and transform only if required
  bufferSize = gltfimage.width * gltfimage.height * 4;
  buffer = new unsigned char[bufferSize];
  unsigned char *rgba = buffer;
  unsigned char *rgb = &gltfimage.image[0];
  for (int32_t i = 0; i < gltfimage.width * gltfimage.height; ++i) {
    for (int32_t j = 0; j < 3; ++j) {
      rgba[j] = rgb[j];
    }
    rgba += 4;
    rgb += 3;
  }
  deleteBuffer = true;
} else {
  buffer = &gltfimage.image[0];
  bufferSize = gltfimage.image.size();
}

VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

VkFormatProperties formatProperties;

int width = gltfimage.width;
int height = gltfimage.height;
int mipLevels =
    static_cast<uint32_t>(floor(log2(std::max(width, height))) + 1.0);

vkGetPhysicalDeviceFormatProperties(physicalDevice->Get(), format,
                                    &formatProperties);
assert(formatProperties.optimalTilingFeatures &
       VK_FORMAT_FEATURE_BLIT_SRC_BIT);
assert(formatProperties.optimalTilingFeatures &
       VK_FORMAT_FEATURE_BLIT_DST_BIT);

StagedBuffer stagedBuffer(bufferSize, buffer);

auto mem = stagedBuffer.GetBufferMemory();
auto buf = stagedBuffer.GetVulkanBuffer();

CreateImage(width, height, format, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            _textureImage);

TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(width),
                  static_cast<uint32_t>(height));
TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

auto imageView = ImageView(_logicalDevice->Get(), _textureImage, format);

_imageView = imageView.Get();
*/
//};

void Texture::CreateTextureImageFromBuffer(FT_Bitmap bitmap) {
  /*
  auto logicalDevice = _serviceLocator->GetService<LogicalDevice>();

  int texWidth = bitmap.width, texHeight = bitmap.rows;

  VkDeviceSize imageSize = texWidth * texHeight;

  StagedBuffer buffer(imageSize, bitmap.buffer);

  auto mem = buffer.GetBufferMemory();
  auto buf = buffer.GetVulkanBuffer();

  CreateImage(texWidth, texHeight, VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
              _textureImage);

  TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(texWidth),
                    static_cast<uint32_t>(texHeight));
  TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  auto imageView =
      ImageView(logicalDevice->Get(), _textureImage, VK_FORMAT_R8_UNORM);

  _imageView = imageView.Get();
  */
}

void Texture::CreateTextureImageFromPixels(unsigned char *pixels, int width,
                                           int height) {

  /*
  #if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
  #elif defined(BUILD_FOR_WINDOWS)
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
  #else
    // auto colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
  #endif

    // stbi_set_flip_vertically_on_load(true);
    VkDeviceSize imageSize = width * height * 4 * sizeof(char);
    if (!pixels) {
      exit(EXIT_FAILURE);
    }

    StagedBuffer buffer(imageSize, pixels);

    // stbi_image_free(pixels);

    auto mem = buffer.GetBufferMemory();
    auto buf = buffer.GetVulkanBuffer();

    CreateImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                _textureImage);

    TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(width),
                      static_cast<uint32_t>(height));
    TransitionImageLayout(_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    auto imageView = ImageView(_logicalDevice->Get(), _textureImage,
  colorFormat); _imageView = imageView.Get();
  }

  #ifdef BUILD_FOR_ANDROID

  void Texture::CreateTextureImage(std::string path,
                                   AAssetManager *assetManager) {
    auto logicalDevice = _serviceLocator->GetService<LogicalDevice>();

    // Read the file:
    AAsset *file =
        AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
    size_t fileLength = AAsset_getLength(file);
    stbi_uc *fileContent = new unsigned char[fileLength];
    AAsset_read(file, fileContent, fileLength);
    AAsset_close(file);

    uint32_t imgWidth, imgHeight, n;
    unsigned char *imageData = stbi_load_from_memory(
        fileContent, fileLength, reinterpret_cast<int *>(&imgWidth),
        reinterpret_cast<int *>(&imgHeight), reinterpret_cast<int *>(&n), 4);
    assert(n == 4);

  #if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
  #elif defined(BUILD_FOR_WINDOWS)
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
  #else
    // auto colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
  #endif

    // stbi_set_flip_vertically_on_load(true);
    VkDeviceSize imageSize = imgWidth * imgHeight * 4;

    StagedBuffer buffer(_serviceLocator, imageSize, imageData);

    // stbi_image_free(pixels);

    auto mem = buffer.GetBufferMemory();
    auto buf = buffer.GetVulkanBuffer();

    CreateImage(imgWidth, imgHeight, colorFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _textureImage, mem);

    TransitionImageLayout(_textureImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    // CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(imgWidth),
    // static_cast<uint32_t>(imgHeight));
    TransitionImageLayout(_textureImage, colorFormat,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    auto imageView = ImageView(_logicalDevice->Get(), _textureImage,
  colorFormat); _imageView = imageView.Get();
*/
}

/**
 * @brief Transition image layout from one format to another
 * @param image VkImage to transition
 * @param format
 * @param oldLayout
 * @param newLayout
 * @return (void)
 */
void Texture::TransitionImageLayout(VkImage image, VkImageLayout oldLayout,
                                    VkImageLayout newLayout) {
  // Assert on parameters
  assert(image != VK_NULL_HANDLE);

  // Create a new command buffer and start one - time recording
  auto commandBuffer = new CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

  commandBuffer->RecordOnce();

  // Create pipeline barrier
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    spdlog::error("Unsupported image layout transition!");
  }

  // Set the pipeline barrier
  vkCmdPipelineBarrier(commandBuffer->Get(), sourceStage, destinationStage, 0,
                       0, nullptr, 0, nullptr, 1, &barrier);

  // End one - time recording and add command buffer to queue
  commandBuffer->EndRecordingOnce();
  // _queueSync.commandBuffers.push_back(commandBuffer->Get());

  auto cmdBuf = commandBuffer->Get();

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;
  vkQueueSubmit(_logicalDevice->GetGraphicQueue(), 1, &submitInfo, nullptr);

  vkDeviceWaitIdle(_logicalDevice->Get());
}

/**
 * @brief Copy image data from a VkBuffer to a VkImage
 * @param buffer VkBuffer to copy from
 * @param image VkImage to copy to
 * @param width image width
 * @param height image height
 * @return (void)
 */
void Texture::CopyBufferToImage(const VkBuffer buffer, const VkImage image,
                                const uint32_t width, const uint32_t height) {
  // Assert on parameters
  assert(buffer != VK_NULL_HANDLE);
  assert(image != VK_NULL_HANDLE);
  assert(width != 0);
  assert(height != 0);

  // Create a new command buffer and start one-time recording
  auto commandBuffer = new CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  commandBuffer->RecordOnce();

  // Make the actual copy
  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};
  vkCmdCopyBufferToImage(commandBuffer->Get(), buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  // End one - time recording and add command buffer to queue
  commandBuffer->EndRecordingOnce();
  // _queueSync.commandBuffers.push_back(commandBuffer->Get());

  auto cmdBuf = commandBuffer->Get();
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;
  vkQueueSubmit(_logicalDevice->GetGraphicQueue(), 1, &submitInfo, nullptr);

  vkDeviceWaitIdle(_logicalDevice->Get());
}

/**
 * @brief Create image
 * @param width width of the image
 * @param height height of the image
 * @param format VkFormat colorformat of the image
 * @param tiling VkImageTiling tiling mode
 * @param usage VkImageUsageFlags usage flags
 * @param image VkImage the image
 * @return (void)
 */
void Texture::CreateImage(const uint32_t width, const uint32_t height,
                          const VkFormat format, const VkImageTiling tiling,
                          const VkImageUsageFlags usage, VkImage &image) {
  // Assert on parameters
  assert(width != 0);
  assert(height != 0);
  assert(format != VK_FORMAT_UNDEFINED);

  VmaAllocationCreateInfo allocCreateInfo = {};
  allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
  allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  // Create the image
  vmaCreateImage(_allocator->Get(), &imageInfo, &allocCreateInfo, &image,
                 &_allocation, nullptr);
}

/**
 * @brief Get the supported color format for current platform
 * @return VkFormat the color format
 */
VkFormat Texture::GetColorFormat() {

  return VK_FORMAT_B8G8R8A8_UNORM;

  // #if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
  //   return VK_FORMAT_R8G8B8A8_UNORM;
  // #elif defined(BUILD_FOR_WINDOWS)
  //   return VK_FORMAT_R8G8B8A8_UNORM;
  // #else
  //   return VK_FORMAT_B8G8R8A8_SRGB;
  // #endif
}

/*
// CubeMap related..
void Texture::TransitionImageLayoutCubeMap(VkImage image, VkFormat format,
VkImageLayout oldLayout, VkImageLayout newLayout, unsigned int mips)
{

    _commandBuffer->RecordOnce();

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mips;
    subresourceRange.layerCount = 6;

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange = subresourceRange;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout ==
VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout ==
VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        _commandBuffer->GetCommandBuffer(),
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    _commandBuffer->EndRecordingOnce();

}
*/

/*
void Texture::CreateTextureImageFromKtx(unsigned char *pixels, unsigned int
width, unsigned int height, int size, int mips, VkFormat format, ktxTexture
*ktxTexture)
{

    StagedBuffer buffer(_serviceLocator, size, pixels);

    // stbi_image_free(pixels);

    auto mem = buffer.GetBufferMemory();
    auto buf = buffer.GetVulkanBuffer();

    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = format;
    imageCreateInfo.mipLevels = mips;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.extent = {width, height, 1};
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT |
VK_IMAGE_USAGE_SAMPLED_BIT;
    // Cube faces count as array layers in Vulkan
    imageCreateInfo.arrayLayers = 6;
    // This flag is required for cube map images
    imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

    if (vkCreateImage(_logicalDevice->Get(), &imageCreateInfo, nullptr,
&_textureImage) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_logicalDevice->Get(), _textureImage,
&memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Utility::FindMemoryTypeIndex(_serviceLocator,
memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(_logicalDevice->Get(), &allocInfo, nullptr,
&_textureImageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(_logicalDevice->Get(), _textureImage,
_textureImageMemory, 0);

    TransitionImageLayoutCubeMap(_textureImage, format,
VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mips);

    _commandBuffer->RecordOnce();

    // Setup buffer copy regions for each face including all of its miplevels
    std::vector<VkBufferImageCopy>
        bufferCopyRegions;
    uint32_t offset = 0;

    for (uint32_t face = 0; face < 6; face++)
    {
        for (uint32_t level = 0; level < mips; level++)
        {
            // Calculate offset into staging buffer for the current mip level
and face ktx_size_t offset; KTX_error_code ret =
ktxTexture_GetImageOffset(ktxTexture, level, 0, face, &offset); assert(ret ==
KTX_SUCCESS); VkBufferImageCopy bufferCopyRegion = {};
            bufferCopyRegion.imageSubresource.aspectMask =
VK_IMAGE_ASPECT_COLOR_BIT; bufferCopyRegion.imageSubresource.mipLevel = level;
            bufferCopyRegion.imageSubresource.baseArrayLayer = face;
            bufferCopyRegion.imageSubresource.layerCount = 1;
            bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >>
level; bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
bufferCopyRegion.imageExtent.depth = 1; bufferCopyRegion.bufferOffset =
offset; bufferCopyRegions.push_back(bufferCopyRegion);
        }
    }

    vkCmdCopyBufferToImage(_commandBuffer->GetCommandBuffer(),
buffer.GetVulkanBuffer(), _textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());

    _commandBuffer->EndRecordingOnce();

    TransitionImageLayoutCubeMap(_textureImage, format,
VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mips);

    // Create sampler
    VkSamplerCreateInfo sampler{};
    sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler.maxAnisotropy = 1.0f;
    sampler.magFilter = VK_FILTER_LINEAR;
    sampler.minFilter = VK_FILTER_LINEAR;
    sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    sampler.addressModeV = sampler.addressModeU;
    sampler.addressModeW = sampler.addressModeU;
    sampler.mipLodBias = 0.0f;
    sampler.compareOp = VK_COMPARE_OP_NEVER;
    sampler.minLod = 0.0f;
    sampler.maxLod = static_cast<float>(mips);
    sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    sampler.maxAnisotropy = 1.0f;

    vkCreateSampler(_logicalDevice->Get(), &sampler, nullptr, &_sampler);

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mips;
    subresourceRange.layerCount = 6;

    // Create image view
    VkImageViewCreateInfo view{};
    view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    // Cube map view type
    view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
    view.format = format;
    view.subresourceRange = subresourceRange;
    view.image = _textureImage;

    if (vkCreateImageView(_logicalDevice->Get(), &view, nullptr, &_imageView)
!= VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }


    // Clean up staging resources
    ktxTexture_Destroy(ktxTexture);
}
 */
