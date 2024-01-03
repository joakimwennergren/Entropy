#include "primitive.hpp"

using namespace Entropy::GLTF;

// Primitive
Primitive::Primitive(uint32_t firstIndex, uint32_t indexCount, uint32_t vertexCount, Material &material) : firstIndex(firstIndex), indexCount(indexCount), vertexCount(vertexCount), material(material)
{
    hasIndices = indexCount > 0;
};

void Primitive::setBoundingBox(glm::vec3 min, glm::vec3 max)
{
    bb.min = min;
    bb.max = max;
    bb.valid = true;
}