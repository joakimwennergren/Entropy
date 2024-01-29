#pragma once

// add the include for glm to get matrices
#include <glm/glm.hpp>

struct PushConstant
{
    glm::mat4 modelMatrix;
    glm::vec4 color;
    glm::vec3 position;
    int textureId;
};
