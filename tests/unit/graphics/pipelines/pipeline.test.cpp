#include <catch2/catch_test_macros.hpp>
#include <graphics/pipelines/pipeline.hpp>

using namespace Entropy::Graphics::Pipelines;

class PipelineTest : public Pipeline
{
public:
    PipelineTest(){};
    inline VkPipelineDynamicStateCreateInfo DynamicState() { return CreateDynamicState(); }
};

// Sunny day
TEST_CASE("Factorials are computed", "[factorial]")
{
    PipelineTest pipeline = PipelineTest();
    VkPipelineDynamicStateCreateInfo dynamicState = pipeline.DynamicState();
}
