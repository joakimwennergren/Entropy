#pragma once

#include "graphics/vulkan/instances/vk_instance.hpp"
#include "services/vk_instance_service.hpp"
#include <graphics/renderers/vulkan_renderer.hpp>
#include <kangaru/kangaru.hpp>
#include <services/buffer_factory_service.hpp>
#include <services/vulkan_render_service.hpp>

using namespace Entropy::Graphics::Renderers;
using namespace Entropy::Factories::Vulkan;

namespace Entropy {
namespace Graphics {
namespace Vulkan {
namespace Drawables {

struct Drawable {

  Drawable() {
    _vulkanRenderer =
        &vulkanContainer.service<Graphics::Services::VulkanRenderService>();
  }

  kgr::container vulkanContainer;
  VulkanRenderer *_vulkanRenderer;
  BufferFactory *_bufferFactory;
};

} // namespace Drawables
} // namespace Vulkan
} // namespace Graphics
} // namespace Entropy