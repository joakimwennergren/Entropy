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
        /**
         * @brief Indicates whether an entity or object has a texture.
         *
         * This variable acts as a flag to determine if a texture is associated
         * with the renderable component of an entity. It is commonly used in
         * rendering pipelines to decide if texture-specific operations should
         * be applied when rendering an object.
         *
         * A value of 1 typically means the entity has a texture,
         * while a value of 0 implies no texture is present.
         */
        uint32_t hasTexture;
    };
}

#endif // ENTROPY_PUSHCONSTANTS_H
