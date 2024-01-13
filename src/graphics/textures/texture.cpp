#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include "texture.hpp"

using namespace Entropy::Graphics::Textures;

Texture::Texture(std::shared_ptr<ServiceLocator> serviceLocator)
{
    _commandBuffer = std::make_unique<CommandBuffer>(serviceLocator);
    _serviceLocator = serviceLocator;

    // Get required depenencies
    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));

    if (!logicalDevice->isValid())
    {
        spdlog::error("Trying to create texture with invalid logical device");
        return;
    }

    _logicalDevice = logicalDevice;
}

Texture::~Texture()
{
    vkDestroyImageView(_logicalDevice->Get(), _imageView, nullptr);
    vkDestroyImage(_logicalDevice->Get(), _textureImage, nullptr);
    vkFreeMemory(_logicalDevice->Get(), _textureImageMemory, nullptr);
}

void Texture::CreateTextureImageFromBuffer(FT_Bitmap bitmap)
{
    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(_serviceLocator->getService("LogicalDevice"));

    int texWidth = bitmap.width, texHeight = bitmap.rows;

    VkDeviceSize imageSize = texWidth * texHeight;

    StagedBuffer buffer(_serviceLocator, imageSize, bitmap.buffer);

    auto mem = buffer.GetBufferMemory();
    auto buf = buffer.GetVulkanBuffer();

    CreateImage(texWidth, texHeight, VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _textureImage, mem);

    TransitionImageLayout(_textureImage, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(_textureImage, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    auto imageView = ImageView(logicalDevice->Get(), _textureImage, VK_FORMAT_R8_UNORM);

    _imageView = imageView.Get();

    hasTexture = true;
}

void Texture::CreateTextureImageFromPixels(unsigned char *pixels, int width, int height)
{
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
    VkDeviceSize imageSize = width * height * 4;
    if (!pixels)
    {
        exit(EXIT_FAILURE);
    }

    StagedBuffer buffer(imageSize, pixels);

    // stbi_image_free(pixels);

    auto mem = buffer.GetBufferMemory();
    auto buf = buffer.GetVulkanBuffer();

    CreateImage(width, height, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _textureImage, mem);

    TransitionImageLayout(_textureImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    TransitionImageLayout(_textureImage, colorFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    _imageView = VulkanContext::CreateImageView(_textureImage, colorFormat);

    hasTexture = true;
    */
}

void Texture::CreateTextureImage(std::string path)
{

    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(_serviceLocator->getService("LogicalDevice"));

    if (!logicalDevice->isValid())
    {
        spdlog::error("Trying to create renderpass with invalid logical device");
        return;
    }

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX)
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
#elif defined(BUILD_FOR_WINDOWS)
    auto colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
#else
    auto colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
#endif

    // stbi_set_flip_vertically_on_load(true);

    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        exit(EXIT_FAILURE);
    }

    StagedBuffer buffer(_serviceLocator, imageSize, pixels);

    stbi_image_free(pixels);

    auto mem = buffer.GetBufferMemory();
    auto buf = buffer.GetVulkanBuffer();

    CreateImage(texWidth, texHeight, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _textureImage, mem);

    TransitionImageLayout(_textureImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(buf, _textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(_textureImage, colorFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    auto imageView = ImageView(logicalDevice->Get(), _textureImage, colorFormat);

    _imageView = imageView.Get();

    hasTexture = true;
}

void Texture::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
{
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

    if (vkCreateImage(_logicalDevice->Get(), &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_logicalDevice->Get(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Utility::FindMemoryTypeIndex(_serviceLocator, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_logicalDevice->Get(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(_logicalDevice->Get(), image, imageMemory, 0);
}

void Texture::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    _commandBuffer->RecordOnce();

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

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
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

void Texture::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    _commandBuffer->RecordOnce();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1};

    vkCmdCopyBufferToImage(_commandBuffer->GetCommandBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    _commandBuffer->EndRecordingOnce();
}
