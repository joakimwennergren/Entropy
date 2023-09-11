#include "primitive_factory.hpp"

using namespace Symbios::Graphics::Factories;

PrimitiveFactory::PrimitiveFactory(std::shared_ptr<Context> context)
{
	_context = context;
}

Quad * PrimitiveFactory::NewQuad()
{
	return new Quad(_context);
}