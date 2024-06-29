
#pragma once

#include "kangaru/service.hpp"
#include "services/device_service.hpp"
#include "services/vk_instance_service.hpp"
#include <graphics/synchronization/synchronizer.hpp>
#include <kangaru/kangaru.hpp>

using namespace Entropy::Graphics::Synchronization;

namespace Entropy {
namespace Graphics {
namespace Services {

struct SynchronizationService
    : kgr::service<Synchronizer, kgr::dependency<DeviceService>> {};

} // namespace Services
} // namespace Graphics
} // namespace Entropy
