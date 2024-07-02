#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct UniformBufferObject
{
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 invView;
    glm::vec2 screen;
};

struct UboDataDynamic {
// glm::vec4 color;
// glm::vec4 colorBorder;
// glm::vec4 colorShadow;
glm::mat4 proj;
glm::mat4 view;
glm::mat4 model;
glm::mat4 invView;
// glm::vec2 position;
// glm::vec2 size;
// glm::vec4 borderRadius;
// int shapeId;
// int time;
};