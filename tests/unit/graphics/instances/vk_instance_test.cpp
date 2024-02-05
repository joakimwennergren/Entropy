#include <catch2/catch_test_macros.hpp>
#include <graphics/instances/vk_instance.hpp>

using namespace Entropy::Graphics::Instances;

TEST_CASE("Vulkan instance can be created", "[VulkanInstance]")
{
    // Create new vulkan instance
    auto vkInstance = VulkanInstance("Entropy tests");

    // Assert
    REQUIRE(vkInstance.Get() != nullptr);
}