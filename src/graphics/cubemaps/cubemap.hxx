#pragma once

#include <vulkan/vulkan.hpp>
#include <graphics/commandbuffers/commandbuffer.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <graphics/devices/logical_device.hpp>
#include <graphics/imageviews/imageview.hpp>
#include <graphics/textures/texture.hpp>
#include <renderables/renderable.hpp>
#include <graphics/commandpools/commandpool.hpp>
#include <gltf/model.hpp>

#include "spdlog/spdlog.h"

#include <ktx.h>
#include <ktxvulkan.h>

using namespace Entropy::Graphics::CommandBuffers;
using namespace Entropy::ServiceLocators;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::Graphics::Devices;
using namespace Entropy::Graphics::ImageViews;
using namespace Entropy::Graphics::Textures;
using namespace Entropy::Graphics::CommandPools;
using namespace Entropy::Renderables;

namespace Entropy
{
    namespace Graphics
    {
        namespace CubeMaps
        {
            class CubeMap : public Renderable
            {
            public:
                CubeMap(std::shared_ptr<ServiceLocator> serviceLocator);
                // Loads a cubemap from a file, uploads it to the device and create all Vulkan resources required to display it
                void LoadCubemap(std::string filename, VkFormat format);
                std::unique_ptr<Model> _model;
                std::shared_ptr<Texture> _texture;

            private:
                std::shared_ptr<ServiceLocator> _serviceLocator;
            };
        }
    }
}
