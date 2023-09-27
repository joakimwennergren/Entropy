#include "uniformbuffer.hpp"

using namespace Symbios::Graphics::Buffers;

UniformBuffer::UniformBuffer(VkDeviceSize bufferSize)
{
    // Store vulkan ctx
    _context = Contexts::Global::GetInstance()->GetVulkanContext();

    this->CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _buffer, _bufferMemory);

    vkMapMemory(_context->GetLogicalDevice(), _bufferMemory, 0, bufferSize, 0, &_mappedMemory);
}

UniformBuffer::~UniformBuffer()
{
}
