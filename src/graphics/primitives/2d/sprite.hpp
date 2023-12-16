#pragma once

#include <graphics/data/vertex.hpp>
#include <graphics/textures/texture.hpp>
#include <graphics/buffers/vertexbuffer.hpp>
#include <renderables/renderable.hpp>
#include <filesystem/filesystem.hpp>

using namespace Entropy::Graphics::Textures;
using namespace Entropy::Renderables;

namespace Entropy
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
                Sprite(std::shared_ptr<ServiceLocator> serviceLocator, std::string path);

                Sprite(unsigned char *pixels, int width, int height);

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

            private:
            };
        }
    }
}
