#include <catch2/catch_test_macros.hpp>

#include <graphics/pipelines/pipeline.hpp>
#include <graphics/renderpasses/renderpass.hpp>
#include "setup.hpp"

using namespace Entropy::Graphics::RenderPasses;
using namespace Entropy::Graphics::Pipelines;

TEST_CASE("Pipeline can be created", "[Pipeline]")
{
    auto serviceLocator = Setup();

    auto renderPass = std::make_shared<RenderPass>(serviceLocator);

    auto pipeline = Pipeline(renderPass, serviceLocator);
}