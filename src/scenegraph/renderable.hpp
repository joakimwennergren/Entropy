
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
#include "global.hpp"

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

            int zIndex = 0;

            std::string name;

            inline bool hasBeenTouched(float x, float y)
            {
                if(x >= (position.x/3.0) && x < ((position.x/1.5) + scale.x * 1.5f) && y >= (((position.y/1.5f)) * -1.0f) - scale.y/2.0 && y <= (((position.y/1.5f)) * -1.0f))
                {
                    return true;
                }
                return false;
            }

            int id = 0;

            bool visible = true;

            float rotationX = 0.0;

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