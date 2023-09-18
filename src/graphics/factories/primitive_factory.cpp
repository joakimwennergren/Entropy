#include "primitive_factory.hpp"

using namespace Symbios::Graphics::Factories;

PrimitiveFactory::PrimitiveFactory()
{
    // Store vulkan ctx
    _context = Global::GetInstance()->GetVulkanContext();
}

Quad * PrimitiveFactory::NewQuad()
{
	return new Quad();
}