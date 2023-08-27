#pragma once

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"

using namespace Symbios::Graphics::Textures;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Quad
            {
            public:
                Quad(std::shared_ptr<Context> context);
                ~Quad();
                inline const std::vector<Vertex> GetVertices() { return this->_vertices; };
                inline const std::vector<uint16_t> GetIndices() { return this->_indices; };

                std::unique_ptr<Texture> texture;
                std::unique_ptr<Buffer> vertexBuffer;
                std::unique_ptr<Buffer> indexBuffer;
                glm::vec3 position;

            private:
                std::shared_ptr<Context> _context;
                const std::vector<Vertex> _vertices = {
                    {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

                const std::vector<uint16_t> _indices = {
                    0, 1, 2, 2, 3, 0};
            };
        }
    }
}