#include <catch2/catch_test_macros.hpp>
#include <graphics/instances/vk_instance.hpp>
#include <graphics/surfaces/surface.hpp>

#if defined(BUILD_FOR_MACOS) || defined(BUILD_FOR_LINUX) || defined(BUILD_FOR_WINDOWS)
#include <GLFW/glfw3.h>
#endif

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::Surfaces;

TEST_CASE("Window surface can't be created with nullptr", "[WindowSurface]")
{
    // Create new vulkan instance
    auto vkInstance = std::make_shared<VulkanInstance>("Entropy tests");

    // Create new window surface
    auto surface = WindowSurface(vkInstance, nullptr);

    // Assert
    REQUIRE(surface.Get() == nullptr);
}

TEST_CASE("Window surface can be created with valid window", "[WindowSurface]")
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
    auto surface = WindowSurface(vkInstance, window);

    // Assert
    REQUIRE(surface.Get() != nullptr);
}

TEST_CASE("Window surface can't be created with invalid vkinstance'", "[WindowSurface]")
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
    auto surface = WindowSurface(nullptr, window);

    // Assert
    REQUIRE(surface.Get() == nullptr);
}

TEST_CASE("Window surface can't be created with both parameters = nullptr'", "[WindowSurface]")
{
    // Create new window surface
    auto surface = WindowSurface(nullptr, nullptr);

    // Assert
    REQUIRE(surface.Get() == nullptr);
}