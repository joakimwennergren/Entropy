
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
#include "vulkancontext.hpp"

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

                Renderable()
                {
                    this->id = rand() % UINT_MAX;
                }

               ~Renderable()
               {
                    #if USE_DEBUG_INFO == 1
                        PLOG_DEBUG << "2DRenderable destructor called!";
                    #endif

                    if(_descriptorSetLayout != nullptr) {
                        vkDestroyDescriptorSetLayout(_context->GetLogicalDevice(), _descriptorSetLayout, nullptr);
                    }
               }

               void UpdateImage()
               {
                    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
                    samplerLayoutBinding.binding = 1;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

                    VkDescriptorSetLayoutBinding texturesLayoutBinding{};
                    texturesLayoutBinding.binding = 2;
                    texturesLayoutBinding.descriptorCount = 1;
                    texturesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                    texturesLayoutBinding.pImmutableSamplers = nullptr;
                    texturesLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

                    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {samplerLayoutBinding,texturesLayoutBinding};
                    VkDescriptorSetLayoutCreateInfo layoutInfo{};
                    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
                    layoutInfo.pBindings = bindings.data();

                    if (vkCreateDescriptorSetLayout(_context->GetLogicalDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
                    {
                        throw std::runtime_error("failed to create descriptor set layout!");
                    }

                    std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);

                    VkDescriptorSetAllocateInfo allocInfo{};
                    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                    allocInfo.descriptorPool = _context->GetDescriptorPool();
                    allocInfo.descriptorSetCount = 1; //MAX_CONCURRENT_FRAMES_IN_FLIGHT;
                    allocInfo.pSetLayouts = layouts.data();

                    if (vkAllocateDescriptorSets(_context->GetLogicalDevice(), &allocInfo, &_descriptorSet) != VK_SUCCESS)
                    {
                        throw std::runtime_error("failed to allocate descriptor sets!");
                    }

                    VkDescriptorImageInfo imageInfo{};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = texture->hasTexture ? texture->GetImageView() : nullptr;
                    imageInfo.sampler = _context->_textureSampler;

                    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[0].dstSet = _descriptorSet;
                    descriptorWrites[0].dstBinding = 2;
                    descriptorWrites[0].dstArrayElement = 0;
                    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                    descriptorWrites[0].descriptorCount = 1;
                    descriptorWrites[0].pImageInfo = &imageInfo;

                    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrites[1].dstSet = _descriptorSet;
                    descriptorWrites[1].dstBinding = 1;
                    descriptorWrites[1].dstArrayElement = 0;
                    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
                    descriptorWrites[1].descriptorCount = 1;
                    descriptorWrites[1].pImageInfo = &imageInfo;

                    vkUpdateDescriptorSets(_context->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
                }

            int zIndex = 0;

            unsigned int id = 0;

            std::string name;

            bool visible = true;

            float rotationX = 0.0;
            int orientation = 1;

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

            inline void Translate(float x, float y)
            {
                this->position = glm::vec3(x, y, 0.0);
            }

            inline void Scale(float s)
            {
                this->scale = glm::vec3(s, s, 0.0);
            }

            inline void Scale2D(float w, float h)
            {
                this->scale = glm::vec3(w, h, 0.0);
            }

            inline void Rotate(float r, int orientation)
            {
                this->orientation = orientation;
                this->rotationX = r;
            }
            inline void ZIndex(int z)
            {
                this->zIndex = z;
            }

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
            std::shared_ptr<Context> _context;
        };
    }
}
