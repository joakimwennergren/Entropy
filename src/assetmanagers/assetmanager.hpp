#pragma once

#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <future>
#include <mutex>
#include <thread>

#include <gltf/model.hpp>
#include <graphics/vulkan/textures/texture.hpp>
#include <servicelocators/servicelocator.hpp>
#include <services/service.hpp>

#include "assetid.hpp"
#include <graphics/vulkan/synchronization/threadsafequeue.hpp>

#define MULTITHREAD 1

using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Services;
using namespace Entropy::GLTF;

struct TextureLoadData {
  std::string m_path;

  int m_width = 0;
  int m_height = 0;
  int m_channels = 0;

  Entropy::GLTF::Model *model;
};

struct TextureLoadJob {
  AssetId materialId;
  std::string texturePath;
  TextureLoadData loadedData;
};

namespace EntropyEditor {
class AssetManager : public Service {
public:
  AssetManager(std::shared_ptr<ServiceLocator> serviceLocator);
  ~AssetManager();

  void Initialize();
  void LoaderThread();
  void Update(float frameTime);

  // Models
  // std::shared_ptr<Model> LoadModel(const std::string &path);

  // void RegisterMaterial(std::shared_ptr<Material> material);
  // void RegisterEntity(std::shared_ptr<Entity> entity);

  // Shaders
  // Shader *LoadShader(const std::string &name, const std::string &vertPath,
  // const std::string &fragPath); Shader *GetShader(const std::string &name);

  // Textures
  std::future<Entropy::GLTF::Model *> LoadTextureAsync(const std::string &path);

private:
  std::shared_ptr<ServiceLocator> _serviceLocator;
  ThreadsafeQueue<TextureLoadJob> m_loadingTexturesQueue;
  ThreadsafeQueue<TextureLoadJob> m_processingTexturesQueue;

  int m_maxThreads = 1;
  std::vector<std::thread> m_workerThreads;
  std::unordered_map<std::thread::id, std::string> m_threadNames;

  std::atomic<bool> m_loadingThreadActive;

  std::mutex m_outputMutex;

  // ---
  static const std::string s_mainAssetDirectory;
  static const std::string s_assetShaderDir;
  static const std::string s_assetModelDir;
  static const std::string s_assetImagesDir;
};
} // namespace EntropyEditor
