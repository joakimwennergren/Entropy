#include "default_pipeline.hpp"

using namespace Symbios::Graphics::Pipeline;

Default::Default(std::shared_ptr<Symbios::Core::Context> context)
{
    // Create Shader
    auto shader = Symbios::Graphics::Shader::Default("/Users/joakim/Desktop/Symbios/shaders/basic/vert.spv", "/Users/joakim/Desktop/Symbios/shaders/basic/frag.spv", context);
    std::make_unique<Symbios::Graphics::Shader::Default>(shader);
}