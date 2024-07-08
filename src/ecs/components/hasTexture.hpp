#pragma once

#include "graphics/vulkan/textures/normal_texture.hpp"
#include <graphics/primitives/2d/line.hpp>

using namespace Entropy::Graphics::Primitives;

namespace Entropy {
namespace Components {
struct HasTexture {
  std::shared_ptr<NormalTexture> texture;
};
} // namespace Components
} // namespace Entropy