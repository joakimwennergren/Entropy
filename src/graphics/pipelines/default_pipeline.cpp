#include "default_pipeline.hpp"

using namespace Symbios::Graphics::Pipeline;

Default::Default()
{
    // Create Shader
    auto shader = Symbios::Graphics::Shader::Default("/Users/joakim/Desktop/Symbios/shaders/basic/vert.spv", "/Users/joakim/Desktop/Symbios/shaders/basic/frag.spv");
    std::make_unique<Symbios::Graphics::Shader::Default>(shader);
}