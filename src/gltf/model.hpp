#pragma once

#include <glm/glm.hpp>
#include "primitive.hpp"

namespace Entropy
{
    namespace GLTF
    {
        struct Model
        {
            struct Vertex
            {
                glm::vec3 pos;
                glm::vec3 normal;
                glm::vec2 uv0;
                glm::vec2 uv1;
                glm::vec4 joint0;
                glm::vec4 weight0;
                glm::vec4 color;
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

            std::vector<Texture> textures;
            std::vector<TextureSampler> textureSamplers;
            std::vector<Material> materials;
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

            void destroy(VkDevice device);
            void loadNode(vkglTF::Node *parent, const tinygltf::Node &node, uint32_t nodeIndex, const tinygltf::Model &model, LoaderInfo &loaderInfo, float globalscale);
            void getNodeProps(const tinygltf::Node &node, const tinygltf::Model &model, size_t &vertexCount, size_t &indexCount);
            void loadSkins(tinygltf::Model &gltfModel);
            void loadTextures(tinygltf::Model &gltfModel, vks::VulkanDevice *device, VkQueue transferQueue);
            VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);
            VkFilter getVkFilterMode(int32_t filterMode);
            void loadTextureSamplers(tinygltf::Model &gltfModel);
            void loadMaterials(tinygltf::Model &gltfModel);
            void loadAnimations(tinygltf::Model &gltfModel);
            void loadFromFile(std::string filename, vks::VulkanDevice *device, VkQueue transferQueue, float scale = 1.0f);
            void drawNode(Node *node, VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);
            void calculateBoundingBox(Node *node, Node *parent);
            void getSceneDimensions();
            void updateAnimation(uint32_t index, float time);
            Node *findNode(Node *parent, uint32_t index);
            Node *nodeFromIndex(uint32_t index);
        };
    }
}