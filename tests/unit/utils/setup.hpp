#pragma once

#include <graphics/instances/vk_instance.hpp>
#include <graphics/surfaces/surface.hpp>
#include <graphics/devices/physical_device.hpp>
#include <graphics/swapchains/swapchain.hpp>
#include <graphics/imageviews/imageview.hpp>
#include <graphics/descriptorpools/descriptorpool.hpp>
#include <graphics/descriptorsetlayouts/descriptorsetlayout.hpp>
#include <graphics/descriptorsets/descriptorset.hpp>
#include <graphics/commandpools/commandpool.hpp>
#include <servicelocators/servicelocator.hpp>
#include <scenegraphs/scenegraph.hpp>
#include <scripting/lua.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::Swapchains;
using namespace Entropy::SceneGraphs;
using namespace Entropy::Scripting;
using namespace Entropy::Graphics::ImageViews;
using namespace Entropy::Graphics::CommandPools;
using namespace Entropy::Graphics::DescriptorPools;
using namespace Entropy::Graphics::DescriptorsetLayouts;
using namespace Entropy::Graphics::Descriptorsets;
using namespace Entropy::ServiceLocators;

std::shared_ptr<ServiceLocator> Setup();