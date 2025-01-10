#ifndef ENTROPY_PUSHCONSTANTS_H
#define ENTROPY_PUSHCONSTANTS_H

#include <cstdint>

namespace Entropy::Data {
    /**
     * @struct PushConstBlock
     * Represents a structure used for Vulkan push constants to transfer a small
     * set of data to the GPU. This is specifically used for instance index
     * management and texture information during rendering.
     */
    struct PushConstBlock {
        /**
         * @brief Represents the index of an instance in the Vulkan renderer pipeline.
         *
         * This member is used to identify a specific renderable instance during the
         * rendering process. The index corresponds to the instance data stored in the
         * GPU buffer and is typically used to map transformation matrices, colors,
         * and other instance-specific properties.
         */
        uint32_t instanceIndex;
    };
}

#endif // ENTROPY_PUSHCONSTANTS_H
