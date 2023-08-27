#pragma once

// add the include for glm to get matrices
#include <glm/glm.hpp>

struct InstancePushConstants
{
    glm::mat4 modelMatrix;
};
