/**
 * Vulkan glTF model and texture loading class based on tinyglTF
 * (https://github.com/syoyo/tinygltf)
 *
 * Copyright (C) 2018-2022 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT)
 * (http://opensource.org/licenses/MIT)
 */

#pragma once

#define NOMINMAX
#ifdef BUILD_FOR_ANDROID
#define TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#endif

#include <config.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <string>
#include <vector>

#include "vulkan/vulkan.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tiny_gltf.h"
#include <filesystem/filesystem.hpp>
#include <graphics/data/vertex.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/buffers/indexbuffer.hpp>
#include <graphics/vulkan/buffers/uniformbuffer.hpp>
#include <graphics/vulkan/buffers/vertexbuffer.hpp>
#include <graphics/vulkan/pipelines/pipeline.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <servicelocators/servicelocator.hpp>
#include <timing/timer.hpp>

#include <stb_image.h>

// Changing this value here also requires changing it in the vertex shader
#define MAX_NUM_JOINTS 128u

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Timing;

#ifdef BUILD_FOR_ANDROID
#include <android/asset_manager.h>
#endif

namespace Entropy {
namespace GLTF {
struct Node;

struct BoundingBox {
  glm::vec3 min;
  glm::vec3 max;
  bool valid = false;
  BoundingBox();
  BoundingBox(glm::vec3 min, glm::vec3 max);
  BoundingBox getAABB(glm::mat4 m);
};

struct TextureSampler {
  VkFilter magFilter;
  VkFilter minFilter;
  VkSamplerAddressMode addressModeU;
  VkSamplerAddressMode addressModeV;
  VkSamplerAddressMode addressModeW;
};

struct Material {
  enum AlphaMode { ALPHAMODE_OPAQUE, ALPHAMODE_MASK, ALPHAMODE_BLEND };
  AlphaMode alphaMode = ALPHAMODE_OPAQUE;
  float alphaCutoff = 1.0f;
  float metallicFactor = 1.0f;
  float roughnessFactor = 1.0f;
  glm::vec4 baseColorFactor = glm::vec4(1.0f);
  glm::vec4 emissiveFactor = glm::vec4(0.0f);
  Texture *baseColorTexture;
  Texture *metallicRoughnessTexture;
  Texture *normalTexture;
  Texture *occlusionTexture;
  Texture *emissiveTexture;
  bool doubleSided = false;
  struct TexCoordSets {
    uint8_t baseColor = 0;
    uint8_t metallicRoughness = 0;
    uint8_t specularGlossiness = 0;
    uint8_t normal = 0;
    uint8_t occlusion = 0;
    uint8_t emissive = 0;
  } texCoordSets;
  struct Extension {
    Texture *specularGlossinessTexture;
    Texture *diffuseTexture;
    glm::vec4 diffuseFactor = glm::vec4(1.0f);
    glm::vec3 specularFactor = glm::vec3(0.0f);
  } extension;
  struct PbrWorkflows {
    bool metallicRoughness = true;
    bool specularGlossiness = false;
  } pbrWorkflows;
  VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
  int index = 0;
  bool unlit = false;
  float emissiveStrength = 1.0f;
};

struct Primitive {
  uint32_t firstIndex;
  uint32_t indexCount;
  uint32_t vertexCount;
  Material &material;
  bool hasIndices;
  BoundingBox bb;
  Primitive(uint32_t firstIndex, uint32_t indexCount, uint32_t vertexCount,
            Material &material);
  void setBoundingBox(glm::vec3 min, glm::vec3 max);
};

struct Mesh {
  std::vector<Primitive *> primitives;
  BoundingBox bb;
  BoundingBox aabb;
  UniformBuffer *tempBuffer;
  struct UniformBuffer {
    VkBuffer buffer;
    VkDeviceMemory memory;
    VkDescriptorBufferInfo descriptor;
    VkDescriptorSet descriptorSet;
    void *mapped;
  } uniformBuffer;
  struct UniformBlock {
    glm::mat4 matrix;
    glm::mat4 jointMatrix[MAX_NUM_JOINTS]{};
    float jointcount{0};
  } uniformBlock;
  Mesh(std::shared_ptr<ServiceLocator> serviceLocator, glm::mat4 matrix);
  ~Mesh();
  void setBoundingBox(glm::vec3 min, glm::vec3 max);
};

struct Skin {
  std::string name;
  Node *skeletonRoot = nullptr;
  std::vector<glm::mat4> inverseBindMatrices;
  std::vector<Node *> joints;
};

struct Node {
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

struct AnimationChannel {
  enum PathType { TRANSLATION, ROTATION, SCALE };
  PathType path;
  Node *node;
  uint32_t samplerIndex;
};

struct AnimationSampler {
  enum InterpolationType { LINEAR, STEP, CUBICSPLINE };
  InterpolationType interpolation;
  std::vector<float> inputs;
  std::vector<glm::vec4> outputsVec4;
};

struct Animation {
  std::string name;
  std::vector<AnimationSampler> samplers;
  std::vector<AnimationChannel> channels;
  float start = std::numeric_limits<float>::max();
  float end = std::numeric_limits<float>::lowest();
};

class Model {
public:
  std::vector<Vertex> vertices_temp;
  std::vector<uint32_t> indices_temp;
  struct Vertices {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory;
  } vertices;
  struct Indices {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory;
  } indices;

  glm::mat4 aabb;

  std::vector<Node *> nodes;
  std::vector<Node *> linearNodes;

  std::vector<Skin *> skins;

  std::vector<Texture *> textures;
  std::vector<TextureSampler> textureSamplers;
  std::vector<Material> materials;
  std::vector<Animation> animations;
  std::vector<std::string> extensions;

  struct Dimensions {
    glm::vec3 min = glm::vec3(FLT_MAX);
    glm::vec3 max = glm::vec3(-FLT_MAX);
  } dimensions;

  struct LoaderInfo {
    uint32_t *indexBuffer;
    Vertex *vertexBuffer;
    size_t indexPos = 0;
    size_t vertexPos = 0;
  };

  Model(std::shared_ptr<ServiceLocator> serviceLocator);
#if defined(BUILD_FOR_ANDROID)
  Model(std::shared_ptr<ServiceLocator> serviceLocator,
        AAssetManager *assetmngr);
#endif

  ~Model() {
    spdlog::info("Destroying model!");
    for (auto &texture : textures) {
      delete texture;
    }
    textures.resize(0);
    textureSamplers.resize(0);
    for (auto node : nodes) {
      delete node;
    }
    materials.resize(0);
    animations.resize(0);
    nodes.resize(0);
    linearNodes.resize(0);
    extensions.resize(0);
    for (auto skin : skins) {
      delete skin;
    }
    skins.resize(0);
    // vertexBuffer->Destroy();
    // indexBuffer->Destroy();

    vertexBuffer.reset();
    indexBuffer.reset();
  }

  void destroy();
  void loadNode(Node *parent, const tinygltf::Node &node, uint32_t nodeIndex,
                const tinygltf::Model &model, LoaderInfo &loaderInfo,
                float globalscale);
  void getNodeProps(const tinygltf::Node &node, const tinygltf::Model &model,
                    size_t &vertexCount, size_t &indexCount);
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
  void SetupCubeMap(std::shared_ptr<Texture> texture);
  void updateAnimation(uint32_t index, float time);
  Node *findNode(Node *parent, uint32_t index);
  Node *nodeFromIndex(uint32_t index);
  void setupNodeDescriptorSet(Node *node, VkDescriptorSetLayout layout);
  void renderNode(Node *node, VkCommandBuffer commandBuffer,
                  std::shared_ptr<Pipeline> pipeline,
                  Material::AlphaMode alphaMode);

  std::unique_ptr<VertexBuffer> vertexBuffer;
  std::unique_ptr<IndexBuffer<uint32_t>> indexBuffer;
  Entropy::Timing::Timer *timer;
  float tick;
  VkDescriptorSet _noTextureDs;

private:
  std::shared_ptr<ServiceLocator> _serviceLocator;

  Texture *noTexture;
  bool isCubeMap = false;
  VkDescriptorSet _cubeMapDS;

#ifdef BUILD_FOR_ANDROID
  AAssetManager *assetManager;
#endif
};
} // namespace GLTF
} // namespace Entropy
