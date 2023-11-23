#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct UniformBufferObject
{
    glm::mat4 view;
    glm::mat4 proj;
};