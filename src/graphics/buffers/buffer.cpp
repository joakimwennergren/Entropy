#include "buffer.hpp"

using namespace Symbios::Graphics::Buffers;

/**
 * @brief Construct a new Buffer:: Buffer object
 *
 * @param context
 */
Buffer::Buffer(std::shared_ptr<Context> context)
{
    _context = context;
}

/**
 * @brief Destroy the Buffer:: Buffer object
 *
 */
Buffer::~Buffer()
{
    vkDestroyBuffer(_context->GetLogicalDevice(), _buffer, nullptr);
    vkFreeMemory(_context->GetLogicalDevice(), _bufferMemory, nullptr);
}

/**
 * @brief
 *
 * @param srcBuffer
 * @param dstBuffer
 * @param size
 */
void Buffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = _context->GetCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_context->GetLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_context->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_context->GetGraphicsQueue());

    vkFreeCommandBuffers(_context->GetLogicalDevice(), _context->GetCommandPool(), 1, &commandBuffer);
}

/**
 * @brief
 *
 * @param vertices
 */
void Buffer::CreateVertexBuffer(std::vector<Vertex> vertices)
{

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(_context->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(_context->GetLogicalDevice(), stagingBufferMemory);

    CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(_context->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(_context->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

/**
 * @brief
 *
 * @param indices
 */
void Buffer::CreateIndexBufferUint16(std::vector<uint16_t> indices)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(_context->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(_context->GetLogicalDevice(), stagingBufferMemory);

    CreateBuffer(_context, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _bufferMemory);

    CopyBuffer(stagingBuffer, _buffer, bufferSize);

    vkDestroyBuffer(_context->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(_context->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

/**
 * @brief
 *
 * @param _context
 * @param size
 * @param usage
 * @param properties
 * @param buffer
 * @param bufferMemory
 */
void Buffer::CreateBuffer(std::shared_ptr<Context> _context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_context->GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_context->GetLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(_context, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_context->GetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(_context->GetLogicalDevice(), buffer, bufferMemory, 0);
}

/**
 * @brief
 *
 * @param _context
 * @param typeFilter
 * @param properties
 * @return uint32_t
 */
uint32_t Buffer::FindMemoryType(std::shared_ptr<Context> _context, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_context->GetPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    PLOG_ERROR << "failed to find suitable memory type!";
    exit(EXIT_FAILURE);
}
