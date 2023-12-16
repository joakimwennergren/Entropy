#include <catch2/catch_test_macros.hpp>
#include <graphics/instances/vk_instance.hpp>
#include <graphics/validationlayers/validationlayer.hpp>

using namespace Entropy::Graphics::Instances;
using namespace Entropy::Graphics::ValidationLayers;

TEST_CASE("Check validation layer support with layers supplied", "[ValidationLayer]")
{
    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    // Check for validation layer support
    auto enabled = ValidationLayer::CheckValidationLayerSupport(validationLayers);

    // Assert
    REQUIRE(enabled == true);
}

TEST_CASE("Check validation layer support with layers not supplied", "[ValidationLayer]")
{
    const std::vector<const char *> validationLayers = {""};

    // Check for validation layer support
    auto enabled = ValidationLayer::CheckValidationLayerSupport(validationLayers);

    // Assert
    REQUIRE(enabled == false);
}