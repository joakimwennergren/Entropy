#pragma once

#include <cstdint>

namespace Entropy::Data {
    struct PushConstBlock {
        uint32_t instanceIndex;
        uint32_t hasTexture;
    };
}
