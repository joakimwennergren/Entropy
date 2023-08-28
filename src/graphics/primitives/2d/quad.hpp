#pragma once

#include "vertex.hpp"
#include "texture.hpp"
#include "context.hpp"
#include "vertexbuffer.hpp"

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
                /**
                 * @brief Construct a new Quad object
                 *
                 */
                Quad() = default;

                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Quad(std::shared_ptr<Context> context);

                /**
                 * @brief Destroy the Quad object
                 *
                 */
                ~Quad();

                /**
                 * @brief Get the Vertices object
                 *
                 * @return const std::vector<Vertex>
                 */
                inline const std::vector<Vertex> GetVertices() { return this->_vertices; };

                /**
                 * @brief Get the Indices object
                 *
                 * @return const std::vector<uint16_t>
                 */
                inline const std::vector<uint16_t> GetIndices() { return this->_indices; };

                std::unique_ptr<Texture> texture;
                std::unique_ptr<VertexBuffer> vertexBuffer;
                std::unique_ptr<Buffer> indexBuffer;
                glm::vec3 position;
                int textureId = -1;
                glm::vec4 color;

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