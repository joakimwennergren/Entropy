#pragma once

#include <glm/glm.hpp>

namespace Entropy::Data {
    /**
     * @class UboDataDynamic
     * @brief A class that manages dynamically sized Uniform Buffer Objects (UBOs).
     *
     * The UboDataDynamic class is designed to manage the allocation, updating,
     * and deallocation of Uniform Buffer Objects (UBOs) within a graphics
     * application. This class supports dynamic resizing to accommodate changes
     * in the amount of data being managed.
     */
    struct UboDataDynamic {
        glm::mat4 perspective;
        glm::mat4 view;
    };

    /**
     * @class InstanceData
     * @brief A class responsible for managing instance-specific data.
     *
     * The InstanceData class is utilized to handle the data associated with
     * individual instances in a graphics application. This typically includes
     * instance transformations, per-instance attributes, and other related data.
     * It ensures that each instance's data is properly stored and accessed.
     */
    struct InstanceData {
        glm::mat4 model;
        glm::vec4 color;
        glm::vec2 resolution;
        int type;
        int _pad[1];
    };
} // namespace Entropy::Data
