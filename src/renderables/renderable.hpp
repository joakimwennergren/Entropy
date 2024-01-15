#pragma once

#include <memory>

#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <graphics/buffers/buffer.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <scripting/script.hpp>

#include <string>
#include <glm/glm.hpp>

using namespace Entropy::Graphics::Textures;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Graphics::DescriptorPools;
using namespace Entropy::Scripting;

namespace Entropy
{
    namespace Renderables
    {
        class Renderable : public std::enable_shared_from_this<Renderable>
        {
        public:
            Renderable()
            {
                this->id = rand() % UINT_MAX;
            }

            void SetScript(std::string script)
            {
                this->script->script = script;
            };

            void SetScriptFile(std::string scriptFile)
            {
                this->script->scriptFile = scriptFile;
                // this->parent = parent;
            };

            ~Renderable()
            {
                /*
                VulkanContext *vkContext = VulkanContext::GetInstance();

                vkDeviceWaitIdle(vkContext->logicalDevice);

                if (_descriptorSet != VK_NULL_HANDLE)
                {
                    vkFreeDescriptorSets(
                        vkContext->logicalDevice,
                        vkContext->descriptorPool,
                        1,
                        &_descriptorSet);
                }

                if (_descriptorSetLayout != VK_NULL_HANDLE)
                {
                    vkDestroyDescriptorSetLayout(vkContext->logicalDevice, _descriptorSetLayout, nullptr);
                }
                */

                delete texture;
            }

            bool isAbleToRender()
            {
                if (this->vertexBuffer == nullptr)
                {
                    return false;
                }

                return true;
            }

            void UpdateImage(std::shared_ptr<ServiceLocator> serviceLocator)
            {
                // Get required depenencies
                auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(serviceLocator->getService("LogicalDevice"));
                auto physicalDevice = std::dynamic_pointer_cast<PhysicalDevice>(serviceLocator->getService("PhysicalDevice"));
                auto descriptorPool = std::dynamic_pointer_cast<DescriptorPool>(serviceLocator->getService("DescriptorPool"));

                if (!logicalDevice->isValid())
                {
                    spdlog::error("Trying to create buffer with invalid logical device");
                    return;
                }

                VkPhysicalDeviceProperties properties{};
                vkGetPhysicalDeviceProperties(physicalDevice->Get(), &properties);

                VkSamplerCreateInfo samplerInfo{};
                samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
                samplerInfo.magFilter = VK_FILTER_LINEAR;
                samplerInfo.minFilter = VK_FILTER_LINEAR;

                samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                samplerInfo.anisotropyEnable = VK_TRUE;
                samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
                samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
                samplerInfo.unnormalizedCoordinates = VK_FALSE;
                samplerInfo.compareEnable = VK_FALSE;
                samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
                samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
                samplerInfo.mipLodBias = 0.0f;
                samplerInfo.minLod = 0.0f;
                samplerInfo.maxLod = 0.0f;

                if (vkCreateSampler(logicalDevice->Get(), &samplerInfo, nullptr, &_textureSampler) != VK_SUCCESS)
                {
                    throw std::runtime_error("failed to create texture sampler!");
                }

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

                std::array<VkDescriptorSetLayoutBinding, 2> bindings = {samplerLayoutBinding, texturesLayoutBinding};
                VkDescriptorSetLayoutCreateInfo layoutInfo{};
                layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
                layoutInfo.pBindings = bindings.data();

                if (vkCreateDescriptorSetLayout(logicalDevice->Get(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
                {
                    throw std::runtime_error("failed to create descriptor set layout!");
                }

                std::vector<VkDescriptorSetLayout> layouts(1, _descriptorSetLayout);

                VkDescriptorSetAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = descriptorPool->Get();
                allocInfo.descriptorSetCount = 1; // MAX_CONCURRENT_FRAMES_IN_FLIGHT;
                allocInfo.pSetLayouts = layouts.data();

                if (vkAllocateDescriptorSets(logicalDevice->Get(), &allocInfo, &_descriptorSet) != VK_SUCCESS)
                {
                    throw std::runtime_error("failed to allocate descriptor sets!");
                }

                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = texture->GetImageView();
                imageInfo.sampler = _textureSampler;

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

                vkUpdateDescriptorSets(logicalDevice->Get(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
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
            int type = 0;

            Texture *texture = nullptr;
            std::unique_ptr<Entropy::Graphics::Buffers::VertexBuffer> vertexBuffer;
            std::unique_ptr<Buffer> indexBuffer;

            // Testing
            VkDescriptorSet _descriptorSet = VK_NULL_HANDLE;
            VkDescriptorSetLayout _descriptorSetLayout = VK_NULL_HANDLE;

            VkSampler _textureSampler;

            inline void Translate2D(float x, float y) { this->position = glm::vec3(x, y, 0.0); }
            inline void Scale2D(float w, float h) { this->scale = glm::vec3(w, h, 0.0); }
            inline void Color(float r, float g, float b, float a) { this->color = glm::vec4(r, g, b, a); };

            inline void Rotate(float r, int orientation)
            {
                this->orientation = orientation;
                this->rotationX = r;
            }

            inline void ZIndex(int z) { this->zIndex = z; }

            inline const std::vector<Vertex> GetVertices() { return this->_vertices; };
            inline const std::vector<uint16_t> GetIndices() { return this->_indices; };

            std::vector<uint16_t> _indices;
            std::vector<Vertex> _vertices;

            std::unique_ptr<Script> script = nullptr;

            std::shared_ptr<Renderable> parent;
            std::vector<std::shared_ptr<Renderable>> children;
        };
    }
}
