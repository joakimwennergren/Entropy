#include "assetmanager.hpp"

#include <string>
#include <chrono>
#include <thread>
#include <future>

using namespace EntropyEditor;

const std::string AssetManager::s_mainAssetDirectory = "../../../../data/";
const std::string AssetManager::s_assetShaderDir = "Shaders/";
const std::string AssetManager::s_assetModelDir = "Objects/";
const std::string AssetManager::s_assetImagesDir = "Images/";

AssetManager::AssetManager(std::shared_ptr<ServiceLocator> serviceLocator)
    : m_loadingThreadActive(true)
{
    _serviceLocator = serviceLocator;
#if MULTITHREAD
    for (int i = 0; i < m_maxThreads; ++i)
    {
        m_workerThreads.push_back(std::thread(&AssetManager::LoaderThread, this));
    }

    m_threadNames.emplace(std::this_thread::get_id(), "Main");

    for (int i = 0; i < m_maxThreads; ++i)
    {
        std::string name = "Thread " + std::to_string(i);
        m_threadNames.emplace(m_workerThreads[i].get_id(), name);
    }
#endif
}

AssetManager::~AssetManager()
{
#if MULTITHREAD
    m_loadingThreadActive = false;
    for (int i = 0; i < m_maxThreads; ++i)
    {
        m_workerThreads[i].join();
    }
#endif
}

void AssetManager::Initialize()
{
}

void AssetManager::LoaderThread()
{
    while (m_loadingThreadActive)
    {
        if (!m_loadingTexturesQueue.size())
        {
            auto assetJob = m_loadingTexturesQueue.pop();

            if (assetJob.has_value())
            {
                auto model = new Model(_serviceLocator);
                model->loadFromFile(assetJob->texturePath);
                assetJob->loadedData.model = model;
                m_processingTexturesQueue.push(assetJob.value());
            }
        }

        unsigned int workLoadSizeRemaining = m_loadingTexturesQueue.size();
        {
            std::scoped_lock<std::mutex> lock(m_outputMutex);
            // std::cout << "[AssetManager] Jobs Queue size: " << workLoadSizeRemaining << '\n';
        }

        const int msToWait = workLoadSizeRemaining > 0 ? 10 : 2000;
        std::this_thread::sleep_for(std::chrono::milliseconds(msToWait));
    }
}

std::future<Entropy::GLTF::Model *> AssetManager::LoadTextureAsync(const std::string &path)
{
    std::cout << "[AssetManager] Creating Simple Texture Asset Job.\n";
    TextureLoadJob job;
    job.texturePath = path;
    job.materialId = AssetId().GetId();
    m_loadingTexturesQueue.push(job);

    // return std::async(std::launch::async, [this]
    //                   {
    //                     while(true)
    //                     {
    //                         auto textureJob = m_processingTexturesQueue.pop();
    //                         if(textureJob.has_value())
    //                         {
    //                             return textureJob.value().loadedData.model;
    //                         }
    //                     } });

    return std::async(std::launch::async, [this, path]
                      { auto model = new Model(_serviceLocator);
                        model->loadFromFile(path);
                        return model; });
}
