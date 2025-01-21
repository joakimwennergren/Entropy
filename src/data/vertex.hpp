#ifndef ENTROPY_VERTEX_H
#define ENTROPY_VERTEX_H

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace Entropy::Data {
    /**
     * @class Vertex
     * @brief Represents a vertex (or node) in a graph.
     *
     * The Vertex class is used in graph data structures to represent the elements within the graph.
     * Each vertex may contain data and can be connected to other vertices via edges.
     */
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv0;
        glm::vec2 uv1;
        glm::vec4 joint0;
        glm::vec4 weight0;
        glm::vec4 color;

        /**
         * @brief Gets the vertex input binding description for a Vulkan graphics pipeline.
         *
         * This function configures and returns a Vulkan vertex input binding description.
         * The binding description specifies details such as the binding index,
         * stride (size of each vertex), and input rate.
         *
         * @return VkVertexInputBindingDescription Configuration describing
         *         how vertex data is bound in the pipeline.
         */
        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        /**
         * @brief Generates an array of vertex input attribute descriptions.
         *
         * This function configures and returns an array of VkVertexInputAttributeDescription
         * structures. Each description specifies various attributes such as binding, location,
         * format, and offset for the vertex shader to properly interpret the vertex data.
         *
         * @return std::array<VkVertexInputAttributeDescription, 7> An array containing the
         *         vertex input attribute descriptions required by the Vulkan graphics pipeline.
         */
        static std::array<VkVertexInputAttributeDescription, 7>
        getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 7> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, normal);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, uv0);

            attributeDescriptions[3].binding = 0;
            attributeDescriptions[3].location = 3;
            attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[3].offset = offsetof(Vertex, uv1);

            attributeDescriptions[4].binding = 0;
            attributeDescriptions[4].location = 4;
            attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[4].offset = offsetof(Vertex, joint0);

            attributeDescriptions[5].binding = 0;
            attributeDescriptions[5].location = 5;
            attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[5].offset = offsetof(Vertex, weight0);

            attributeDescriptions[6].binding = 0;
            attributeDescriptions[6].location = 6;
            attributeDescriptions[6].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[6].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };
} // namespace Entropy::Data

#endif // ENTROPY_VERTEX_H
