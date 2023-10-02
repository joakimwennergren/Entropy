
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
            
            void UpdateImage(VkDescriptorSet ds)
            {

                /*
                VkDescriptorSetLayoutBinding texturesLayoutBinding{};
                texturesLayoutBinding.binding = 2;
                texturesLayoutBinding.descriptorCount = 1;
                texturesLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                texturesLayoutBinding.pImmutableSamplers = nullptr;
                texturesLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

                std::array<VkDescriptorSetLayoutBinding, 1> bindings = {texturesLayoutBinding};
                VkDescriptorSetLayoutCreateInfo layoutInfo{};
                layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
                layoutInfo.pBindings = bindings.data();

                if (vkCreateDescriptorSetLayout(_context->GetLogicalDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
                {
                    throw std::runtime_error("failed to create descriptor set layout!");
                }

                std::vector<VkDescriptorSetLayout> layouts(MAX_CONCURRENT_FRAMES_IN_FLIGHT, _descriptorSetLayout);

                VkDescriptorSetAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = _context->GetDescriptorPool();
                allocInfo.descriptorSetCount = MAX_CONCURRENT_FRAMES_IN_FLIGHT;
                allocInfo.pSetLayouts = layouts.data();

                if (vkAllocateDescriptorSets(_context->GetLogicalDevice(), &allocInfo, &_descriptorSet) != VK_SUCCESS)
                {
                    throw std::runtime_error("failed to allocate descriptor sets!");
                }
                */

                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = texture->GetImageView();
                imageInfo.sampler = texture->GetSampler();

                std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = ds;
                descriptorWrites[0].dstBinding = 1;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pImageInfo = &imageInfo;

                vkUpdateDescriptorSets(_context->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

            }

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
            std::shared_ptr<Context> _context;
        };
    }
}