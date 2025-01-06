#pragma once

#include <cameras/camera_manager.hpp>
#include <ecs/world.hpp>
#include <graphics/vulkan/commandpools/commandpool.hpp>
#include <graphics/vulkan/descriptorpools/descriptorpool.hpp>
#include <graphics/vulkan/devices/logical_device.hpp>
#include <graphics/vulkan/devices/physical_device.hpp>
#include <graphics/vulkan/instances/vk_instance.hpp>
#include <graphics/vulkan/memory/allocator.hpp>
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <graphics/vulkan/swapchains/swapchain.hpp>
#include <physics/2d/physics2d.hpp>
#include <physics/3d/physics3d.hpp>
#include <scripting/lua.hpp>
#include <timing/timer.hpp>
#include <config.hpp>

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::ImageViews;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Cameras;
using namespace Entropy::Timing;
using namespace Entropy::Scripting;
using namespace Entropy::Physics;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::ECS;
