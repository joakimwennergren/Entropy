/**
 * @file easing.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "texture.hpp"
#include "vertexbuffer.hpp"
#include "buffer.hpp"

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Graphics::Buffers;

namespace Symbios
{
    namespace Renderables
    {
        class Renderable
        {
        public:

            int type = -1;

            glm::vec3 position = glm::vec3(0.0);
            glm::vec3 rotation = glm::vec3(1.0);
            glm::vec3 scale = glm::vec3(1.0);
            glm::vec4 color = glm::vec4(1.0);

            int textureId = -1;

            Texture *texture = nullptr;
            std::unique_ptr<VertexBuffer> vertexBuffer;
            std::unique_ptr<Buffer> indexBuffer;

            // Testing
            VkDescriptorSet _descriptorSet;
            VkDescriptorSetLayout _descriptorSetLayout;

            /**
             * @brief Get the Vertices object
             *
             * @return const std::vector<Vertex>
             */
            inline const std::vector<Vertex> GetVertices() { return this->_vertices; };

            /**
             * @brief Get the Indices object
             *
             * @return const std::vector<uint16_t>
             */
            inline const std::vector<uint16_t> GetIndices() { return this->_indices; };


            std::vector<uint16_t> _indices;
            std::vector<Vertex> _vertices;
        };
    }
}