
#pragma once

#include "kangaru/service.hpp"
#include "services/allocator_service.hpp"
#include <graphics/buffers/buffer.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Buffers;

namespace Entropy {
namespace Graphics {
namespace Services {

struct BufferService : kgr::service<Buffer, kgr::dependency<AllocatorService>> {
};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
