#include "quad.hpp"

using namespace Symbios::Graphics::Primitives;

Quad::Quad(std::shared_ptr<Context> context)
{
    _context = context;

    texture = std::make_unique<Texture>(_context);

    // Create buffers @todo temp!!!
    vertexBuffer = std::make_unique<VertexBuffer>(_context, _vertices);

    indexBuffer = std::make_unique<Buffer>(context);
    indexBuffer->CreateIndexBufferUint16(_indices);

    VkDescriptorSetLayoutBinding texturesLayoutBinding{};
    texturesLayoutBinding.binding = 2;
    texturesLayoutBinding.descriptorCount = 1;
    texturesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    texturesLayoutBinding.pImmutableSamplers = nullptr;
    texturesLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 1> bindings = {texturesLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(_context->GetLogicalDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    std::vector<VkDescriptorSetLayout> layouts(MAX_CONCURRENT_FRAMES_IN_FLIGHT, _descriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _context->GetDescriptorPool();
    allocInfo.descriptorSetCount = MAX_CONCURRENT_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(_context->GetLogicalDevice(), &allocInfo, &_descriptorSet) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

Quad::~Quad()
{
}