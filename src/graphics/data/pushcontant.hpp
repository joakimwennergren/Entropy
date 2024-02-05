#pragma once

// add the include for glm to get matrices
#include <glm/glm.hpp>

struct PushConstant
{
    glm::mat4 model;
};
