#pragma once

// include Lua headers
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"
#include "vertexbuffer.hpp"
#include "state.hpp"

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Scripting::States;
using namespace luabridge;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Quad
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 */
                Quad() = default;

                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Quad(std::shared_ptr<Context> context);

                /**
                 * @brief Destroy the Quad object
                 *
                 */
                ~Quad();

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

                Texture *texture = nullptr;
                std::unique_ptr<VertexBuffer> vertexBuffer;
                std::unique_ptr<Buffer> indexBuffer;
                glm::vec3 position;
                glm::vec3 rotation;
                glm::vec3 scale = glm::vec3(1.0);
                int textureId = -1;
                glm::vec4 color;

                void UpdateImage()
                {
                    VkDescriptorImageInfo imageInfo{};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = texture->GetImageView();
                    imageInfo.sampler = _context->_textureSampler;

                    std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[0].dstSet = _descriptorSet;
                    descriptorWrites[0].dstBinding = 2;
                    descriptorWrites[0].dstArrayElement = 0;
                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                    descriptorWrites[0].descriptorCount = 1;
                    descriptorWrites[0].pImageInfo = &imageInfo;

                    vkUpdateDescriptorSets(_context->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
                }

                void BindToLua();

                void Test() { PLOG_ERROR << "KYK"; };

                void SetPosition(glm::vec3 newPosition) { this->position = newPosition; };

                // Testing
                VkDescriptorSet _descriptorSet;
                VkDescriptorSetLayout _descriptorSetLayout;

                std::vector<Vertex> _vertices = {
                    {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

            private:
                std::shared_ptr<Context> _context;

                const std::vector<uint16_t> _indices = {
                    0, 1, 2, 2, 3, 0};
            };
        }
    }
}