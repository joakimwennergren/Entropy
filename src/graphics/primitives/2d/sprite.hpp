#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <contexts/context.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>
#include <global/scenegraph.hpp>
#include <filesystem/filesystem.hpp>
#include <global/vulkancontext.hpp>

using namespace Symbios::Graphics::Textures;
using namespace Entropy::Global;
using namespace Symbios::Renderables;

namespace Symbios
{
    namespace Graphics
    {
        namespace Primitives
        {
            class Sprite : public Renderable
            {
            public:
                Sprite();

                Sprite(FT_Bitmap bitmap);
                /**
                 * @brief Construct a new Quad object
                 *
                 * @param context
                 */
                Sprite(std::string path);

                void New(std::string path, glm::vec3 position, glm::vec3 scale, glm::vec4 color)
                {
                    this->position = position;
                    this->textureId = 1;
                    this->color = color;
                    this->scale = scale;
                    this->texture->CreateTextureImage(path);
                }

                inline void SetName(std::string name) { this->name = name; };
                inline void SetId(int id) { this->id = id; };
                inline void SetPosition(float x, float y) { this->position = glm::vec3(x, y, 0.0); };
                inline void SetScale(float x, float y) { this->scale = glm::vec3(x, y, 0.0); };
                inline void SetZIndex(int z) { this->zIndex = z; };
                inline void SetRotation(float r, int orientation)
                {
                    this->rotationX = r;
                    this->orientation = orientation;
                };
                inline void SetColor(float r, float g, float b, float a) { this->color = glm::vec4(r, g, b, a); };

                std::vector<Vertex> _vertices = {
                    {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
                    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

            private:
            };
        }
    }
}
