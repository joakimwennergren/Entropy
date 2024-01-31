#pragma once

#define NOMINMAX

#include <vector>
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

#include "tiny_gltf.h"

using namespace Entropy::Materials;
using namespace Entropy::Graphics::Buffers;
using namespace Entropy::Renderables;
using namespace Entropy::GLTF;

namespace Entropy
{
    namespace GLTF
    {
        class Model : public Renderable
        {
        public:
            struct Node;

            struct Skin
            {
                std::string name;
                Node *skeletonRoot = nullptr;
                std::vector<glm::mat4> inverseBindMatrices;
                std::vector<Node *> joints;
            };

            struct AnimationChannel
            {
                enum PathType
                {
                    TRANSLATION,
                    ROTATION,
                    SCALE
                };
                PathType path;
                Node *node;
                uint32_t samplerIndex;
            };

            struct AnimationSampler
            {
                enum InterpolationType
                {
                    LINEAR,
                    STEP,
                    CUBICSPLINE
                };
                InterpolationType interpolation;
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
            };

            struct Node
            {
                Node *parent;
                uint32_t index;
                std::vector<Node *> children;
                glm::mat4 matrix;
                std::string name;
                Mesh *mesh;
                Skin *skin;
                int32_t skinIndex = -1;
                glm::vec3 translation{};
                glm::vec3 scale{1.0f};
                glm::quat rotation{};
                BoundingBox bvh;
                BoundingBox aabb;
                glm::mat4 localMatrix();
                glm::mat4 getMatrix();
                void update();
                ~Node();
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
            std::vector<Skin *> skins;

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
            void loadNode(Node *parent, const tinygltf::Node &node, uint32_t nodeIndex, const tinygltf::Model &model, LoaderInfo &loaderInfo, float globalscale);
            void getNodeProps(const tinygltf::Node &node, const tinygltf::Model &model, size_t &vertexCount, size_t &indexCount);
            void loadSkins(tinygltf::Model &gltfModel);
            void loadTextures(tinygltf::Model &gltfModel);
            VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);
            VkFilter getVkFilterMode(int32_t filterMode);
            void loadTextureSamplers(tinygltf::Model &gltfModel);
            void loadMaterials(tinygltf::Model &gltfModel);
            void loadAnimations(tinygltf::Model &gltfModel);
            void loadFromFile(std::string filename, float scale = 1.0f);
            void drawNode(Node *node, VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);
            void calculateBoundingBox(Node *node, Node *parent);
            void getSceneDimensions();
            void updateAnimation(uint32_t index, float time);
            Node *findNode(Node *parent, uint32_t index);
            Node *nodeFromIndex(uint32_t index);

            std::shared_ptr<ServiceLocator> _serviceLocator;
        };
    }
}