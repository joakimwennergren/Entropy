#pragma once

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"
#include "vertexbuffer.hpp"
#include "state.hpp"
#include "renderable.hpp"

using namespace Symbios::Graphics::Textures;
using namespace Symbios::Scripting::States;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Sprite: public Renderable
            {
            public:
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Sprite();

                /**
                 * @brief Destroy the Quad object
                 *
                 */
                ~Sprite();

                void UpdateImage()
                {

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


                    VkDescriptorImageInfo imageInfo{};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = texture->hasTexture ? texture->GetImageView() : nullptr;
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

                std::vector<Vertex> _vertices = {
                    {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

            private:
                std::shared_ptr<Context> _context;

            };
        }
    }
}
