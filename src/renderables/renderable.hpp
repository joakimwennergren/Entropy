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
                id = rand() % UINT_MAX;
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
                auto logicalDevice = std::dynamic_pointer_cast<LogicalDevice>(_serviceLocator->getService("LogicalDevice"));
                auto descriptorPool = std::dynamic_pointer_cast<DescriptorPool>(_serviceLocator->getService("DescriptorPool"));

                vkDeviceWaitIdle(logicalDevice->Get());

                if (_descriptorSet != VK_NULL_HANDLE)
                {
                    vkFreeDescriptorSets(
                        logicalDevice->Get(),
                        descriptorPool->Get(),
                        1,
                        &_descriptorSet);
                }

                if (_descriptorSetLayout != VK_NULL_HANDLE)
                {
                    vkDestroyDescriptorSetLayout(logicalDevice->Get(), _descriptorSetLayout, nullptr);
                }
                */

                if (texture != nullptr)
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

            int zIndex = 0;
            unsigned int id = 0;
            std::string name;

            bool visible = true;
            bool overflowHidden = false;

            float rotation = 0.0;
            glm::vec3 orientation = glm::vec3(1.0f);

            glm::vec3 position = glm::vec3(0.0);
            glm::vec3 scale = glm::vec3(1.0);
            glm::vec4 color = glm::vec4(1.0);
            glm::vec4 colorBorder;
            glm::vec4 colorShadow;
            glm::vec2 position2d;
            glm::vec2 size;
            glm::vec4 borderRadius;

            int textureId = -1;
            int type = 0;

            Texture *texture = nullptr;
            std::unique_ptr<Entropy::Graphics::Buffers::VertexBuffer> vertexBuffer;
            std::unique_ptr<Buffer> indexBuffer;

            VkSampler _textureSampler;

            inline void Translate2D(float x, float y) { this->position = glm::vec3(x, y, 0.0); };
            inline void Translate3D(float x, float y, float z) { position = glm::vec3(x, y, z); };
            inline void Scale2D(float w, float h) { this->scale = glm::vec3(w, h, 0.1); };
            inline void Scale3D(float w, float h, float z) { scale = glm::vec3(w, h, z); };
            inline void Color(float r, float g, float b, float a) { this->color = glm::vec4(r, g, b, a); };

            virtual void Test() = 0;

            inline void ZIndex(int z) { this->zIndex = z; }

            inline const std::vector<Vertex> GetVertices() { return this->_vertices; };
            inline const std::vector<uint16_t> GetIndices() { return this->_indices; };

            std::vector<uint16_t> _indices;
            std::vector<Vertex> _vertices;

            std::unique_ptr<Script> script = nullptr;

            std::shared_ptr<Renderable> parent;
            std::vector<std::shared_ptr<Renderable>> children;

            std::shared_ptr<ServiceLocator> _serviceLocator;
        };
    }
}
