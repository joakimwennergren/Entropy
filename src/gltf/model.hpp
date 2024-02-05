#pragma once

#define NOMINMAX

#include <vector>
#include <memory>
#include <limits>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include "primitive.hpp"
#include "texturesampler.hpp"
#include "mesh.hpp"
#include "bounding_box.hpp"
#include <materials/pbr_material.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/buffers/storagebuffer.hpp>
#include "tiny_gltf.h"
#include <graphics/descriptorpools/descriptorpool.hpp>

using namespace Entropy::Materials;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Renderables;
using namespace Entropy::GLTF;
using namespace Entropy::Graphics::DescriptorPools;

namespace Entropy
{
    namespace GLTF
    {

        class Node
        {
        public:
            Node() = default;
            Node *parent;
            uint32_t index;
            std::vector<Node *> children;
            Mesh mesh;
            glm::vec3 translation{};
            glm::vec3 scale{1.0f};
            glm::quat rotation{};
            int32_t skin = -1;
            glm::mat4 matrix;
            glm::mat4 getLocalMatrix();
        };

        class Model : public Renderable
        {
        public:
            struct Skin
            {
                std::string name;
                Node *skeletonRoot = nullptr;
                std::vector<glm::mat4> inverseBindMatrices;
                std::vector<Node *> joints;
                std::unique_ptr<StorageBuffer> ssbo;
                VkDescriptorSet descriptorSet;
            };

            struct AnimationChannel
            {
                enum PathType
                {
                    TRANSLATION,
                    ROTATION,
                    SCALE
                };
                std::string path;
                Node *node;
                uint32_t samplerIndex;
            };

            uint32_t activeAnimation = 0;

            struct AnimationSampler
            {
                enum InterpolationType
                {
                    LINEAR,
                    STEP,
                    CUBICSPLINE
                };
                std::string interpolation;
                std::vector<float> inputs;
                std::vector<glm::vec4> outputsVec4;
            };

            struct Animation
            {
                std::string name;
                std::vector<AnimationSampler> samplers;
                std::vector<AnimationChannel> channels;
                float start = std::numeric_limits<float>::max();
                float end = std::numeric_limits<float>::lowest();
                float currentTime = 0.0f;
            };

            struct Vertices
            {
                VkBuffer buffer = VK_NULL_HANDLE;
                VkDeviceMemory memory;
            } vertices;

            struct Indices
            {
                VkBuffer buffer = VK_NULL_HANDLE;
                VkDeviceMemory memory;
            } indices;

            glm::mat4 aabb;

            std::vector<Node *> nodes;
            std::vector<Node *> linearNodes;
            std::vector<Skin> skins;

            std::vector<Texture *> textures;
            std::vector<TextureSampler> textureSamplers;
            std::vector<PBRMaterial> materials;
            std::vector<Animation> animations;
            std::vector<std::string> extensions;

            struct Dimensions
            {
                glm::vec3 min = glm::vec3(FLT_MAX);
                glm::vec3 max = glm::vec3(-FLT_MAX);
            } dimensions;

            struct LoaderInfo
            {
                uint32_t *indexBuffer;
                Vertex *vertexBuffer;
                size_t indexPos = 0;
                size_t vertexPos = 0;
            };

            std::unique_ptr<VertexBuffer> _vertexBuffer;
            std::unique_ptr<Buffer> _indexBuffer;
            std::vector<Vertex> vertices_temp;

            Model(std::shared_ptr<ServiceLocator> serviceLocator);
            void destroy(VkDevice device);
            void loadNode(const tinygltf::Node &inputNode, const tinygltf::Model &input, Node *parent, uint32_t nodeIndex, std::vector<uint32_t> &indexBuffer, std::vector<Vertex> &vertexBuffer);
            void loadSkins(tinygltf::Model &gltfModel);
            void loadTextures(tinygltf::Model &input);
            void loadImages(tinygltf::Model &input);
            VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);
            VkFilter getVkFilterMode(int32_t filterMode);
            void loadTextureSamplers(tinygltf::Model &gltfModel);
            void loadMaterials(tinygltf::Model &gltfModel);
            void loadAnimations(tinygltf::Model &gltfModel);
            void LoadFromFile(std::string filename, float size);
            void drawNode(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, Node *node);
            void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);
            void updateAnimation(float deltaTime);
            Node *findNode(Node *parent, uint32_t index);
            Node *nodeFromIndex(uint32_t index);
            void updateJoints(Node *node);
            std::shared_ptr<ServiceLocator> _serviceLocator;
        };
    }
}