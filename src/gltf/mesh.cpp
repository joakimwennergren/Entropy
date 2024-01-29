#include "mesh.hpp"

using namespace Entropy::GLTF;

Mesh::Mesh(std::shared_ptr<ServiceLocator> serviceLocator, glm::mat4 matrix)
{
    _logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));

    _buffer = std::make_unique<UniformBuffer>(serviceLocator, (VkDeviceSize)sizeof(uniformBlock));

    this->uniformBlock.matrix = matrix;
};

Mesh::~Mesh()
{
    vkDestroyBuffer(_logicalDevice->Get(), _buffer->GetVulkanBuffer(), nullptr);
    vkFreeMemory(_logicalDevice->Get(), _buffer->GetBufferMemory(), nullptr);
    for (Primitive *p : primitives)
        delete p;
}

void Mesh::setBoundingBox(glm::vec3 min, glm::vec3 max)
{
    bb.min = min;
    bb.max = max;
    bb.valid = true;
}