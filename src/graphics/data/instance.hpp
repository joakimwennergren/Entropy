#pragma once

#include <array>

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct InstanceUBO
{
    glm::mat4 *model = nullptr;
};