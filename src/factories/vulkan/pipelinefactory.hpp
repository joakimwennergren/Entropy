#pragma once
#include "../../factories/vulkan/descriptorset_factory.hpp"
#include "../../factories/vulkan/descriptorsetlayout_factory.hpp"
#include "../../graphics/vulkan/descriptorpools/descriptorpool.hpp"
#include <graphics/vulkan/pipelinecaches/pipelinecache.hpp>
#include <graphics/vulkan/pipelines/static_pipeline.hpp>

using namespace Entropy::Graphics::Vulkan;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Graphics::Vulkan::Pipelines;

namespace Entropy
{
  namespace Factories
  {
    namespace Vulkan
    {

      struct PipelineFactory
      {

        PipelineFactory() {}

        StaticPipeline *CreateStaticPipeline()
        {
          return new StaticPipeline();
        }

      private:
      };

    } // namespace Vulkan
  } // namespace Factories
} // namespace Entropy