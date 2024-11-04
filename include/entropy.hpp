#pragma once

#include "../src/cameras/camera_manager.hpp"
#include "../src/ecs/world.hpp"
#include "../src/graphics/vulkan/commandpools/commandpool.hpp"
#include "../src/graphics/vulkan/descriptorpools/descriptorpool.hpp"
#include "../src/graphics/vulkan/devices/logical_device.hpp"
#include "../src/graphics/vulkan/devices/physical_device.hpp"
#include "../src/graphics/vulkan/instances/vk_instance.hpp"
#include "../src/graphics/vulkan/memory/allocator.hpp"
#include "../src/graphics/vulkan/pipelinecaches/pipelinecache.hpp"
#include "../src/graphics/vulkan/swapchains/swapchain.hpp"
#include "../src/physics/2d/physics2d.hpp"
#include "../src/physics/3d/physics3d.hpp"
#include "../src/scripting/lua.hpp"
#include "../src/timing/timer.hpp"
#include "config.hpp"

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::Swapchains;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorsetLayouts;
using namespace Entropy::Graphics::Vulkan::Descriptorsets;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Cameras;
using namespace Entropy::Timing;
using namespace Entropy::Scripting;
using namespace Entropy::Physics;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::ECS;
