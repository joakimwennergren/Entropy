#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct MatrixUniforms
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};