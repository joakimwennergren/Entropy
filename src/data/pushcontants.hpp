#pragma once

// add the include for glm to get matrices
#include <glm/glm.hpp>

struct PushConstBlock
{
    glm::vec2 scale;
    glm::vec2 translate;
};
