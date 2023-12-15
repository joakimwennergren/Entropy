#include <catch2/catch_test_macros.hpp>

#include <config.hpp>

#include <graphics/instances/vk_instance.hpp>
#include <graphics/surfaces/surface.hpp>
#include <graphics/devices/physical_device.hpp>
#include <graphics/devices/logical_device.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
#include <GLFW/glfw3.h>
#endif

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;
using namespace Entropy::Graphics::Devices;

TEST_CASE("Logical device can be created", "[LogicalDevice]")
{
    // Create new vulkan instance
    auto vkInstance = std::make_shared<VulkanInstance>("Entropy tests");

    // Initialize GLFW
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto window = glfwCreateWindow(640, 480, "Entropy tests", NULL, NULL);

    // Create new window surface
    auto surface = std::make_shared<WindowSurface>(vkInstance, window);

    // Create physical device
    auto physicalDevice = std::make_shared<PhysicalDevice>(vkInstance, surface);

    // Create logical device
    auto logicalDevice = LogicalDevice(physicalDevice, surface);

    // Assert
    REQUIRE(logicalDevice.Get() != nullptr);
}