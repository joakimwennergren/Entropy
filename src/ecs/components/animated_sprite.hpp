#pragma once

#include "graphics/vulkan/textures/texture.hpp"
#include <graphics/primitives/2d/line.hpp>

using namespace Entropy::Graphics::Primitives;

namespace Entropy {
namespace Components {
struct HasAnimatedSprite {
  std::vector<std::shared_ptr<Texture>> textures;
};
} // namespace Components
} // namespace Entropy
