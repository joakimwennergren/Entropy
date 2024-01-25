#include "sprite.hpp"

using namespace Entropy::Graphics::Primitives;

Sprite::Sprite()
{
    /*
    _indices = {
        0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture();

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(_vertices);
    indexBuffer = std::make_unique<Buffer>();

    indexBuffer->CreateIndexBufferUint16(_indices);
    */
}

Sprite::Sprite(std::shared_ptr<ServiceLocator> serviceLocator, FT_Bitmap bitmap)
{
    _serviceLocator = serviceLocator;

    script = std::make_unique<Script>();

    _indices = {
        0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture(serviceLocator);

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(serviceLocator, _vertices);
    indexBuffer = std::make_unique<Buffer>();
    indexBuffer->CreateIndexBufferUint16(serviceLocator, _indices);

    texture->CreateTextureImageFromBuffer(bitmap);

    UpdateDescriptorSets();
}

Sprite::Sprite(std::shared_ptr<ServiceLocator> serviceLocator, std::string path)
{
    _serviceLocator = serviceLocator;

    script = std::make_unique<Script>();

    _indices = {0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture(serviceLocator);

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(serviceLocator, _vertices);

    indexBuffer = std::make_unique<Buffer>();
    indexBuffer->CreateIndexBufferUint16(serviceLocator, _indices);

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->textureId = 1;
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->scale = glm::vec3(1.0, 1.0, 1.0);
    this->texture->CreateTextureImage(path);

    UpdateDescriptorSets();

    type = 1;
}

Sprite::Sprite(unsigned char *pixels, int width, int height)
{
    /*
    _indices = {
        0, 1, 2, 2, 3, 0};

    _vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    texture = new Texture();

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(_vertices);

    indexBuffer = std::make_unique<Buffer>();
    indexBuffer->CreateIndexBufferUint16(_indices);

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->textureId = 1;
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->scale = glm::vec3(1.0, 1.0, 1.0);
    this->texture->CreateTextureImageFromPixels(pixels, width, height);

    UpdateImage();
    */
}

void Sprite::UpdateDescriptorSets()
{
    // Get required depenencies
    auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(_serviceLocator->getService("LogicalDevice"));
    auto physicalDevice = std::dynamic_pointer_cast<PhysicalDevice>(_serviceLocator->getService("PhysicalDevice"));
    auto descriptorPool = std::dynamic_pointer_cast<DescriptorPool>(_serviceLocator->getService("DescriptorPool"));

    if (!logicalDevice->isValid())
    {
        spdlog::error("Trying to create buffer with invalid logical device");
        return;
    }

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice->Get(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(logicalDevice->Get(), &samplerInfo, nullptr, &_textureSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding texturesLayoutBinding{};
    texturesLayoutBinding.binding = 2;
    texturesLayoutBinding.descriptorCount = 1;
    texturesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    texturesLayoutBinding.pImmutableSamplers = nullptr;
    texturesLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {samplerLayoutBinding, texturesLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(logicalDevice->Get(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool->Get();
    allocInfo.descriptorSetCount = 1; // MAX_CONCURRENT_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo, &_descriptorSet) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture->GetImageView();
    imageInfo.sampler = _textureSampler;

    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = _descriptorSet;
    descriptorWrites[0].dstBinding = 2;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pImageInfo = &imageInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = _descriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}