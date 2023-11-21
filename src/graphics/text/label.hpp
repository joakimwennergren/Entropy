#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/ftglyph.h>

#include <glm/glm.hpp>

#include <graphics/primitives/2d/sprite.hpp>
#include <filesystem/filesystem.hpp>
#include <global/vulkancontext.hpp>
#include <renderables/renderable.hpp>
#include <global/scenegraph.hpp>

using namespace Symbios::Renderables;
using namespace Symbios::Graphics::Primitives;
using namespace Symbios;

namespace Symbios
{
    namespace Text
    {
        struct Character
        {
            FT_Bitmap bitmap;
            glm::ivec2 Size;
            glm::ivec2 Bearing;
            FT_Pos Advance;
            FT_Pos YAdvance;
        };

        class Label : public Renderable
        {
        public:
            Label();
            ~Label();
            std::vector<std::shared_ptr<Sprite>> sprites;

            inline void SetPosition(float x, float y)
            {
                if (this->text.length() == 0)
                    return;

                std::string::const_iterator c;
                int cnt = 0;

                for (c = text.begin(); c != text.end(); c++)
                {
                    Character &ch = _characters[*c];

                    float xpos = x + ch.Size.x;
                    float ypos = y - (ch.Bearing.y);

                    /*
                    auto g = this->children[cnt];
                    g->position = glm::vec3(xpos, ypos, 0.0);

                    x += (ch.Advance >> 6) * 2.0f;
                    cnt++;
                    */
                }
            }

            inline void SetText(std::string text)
            {
                this->children.clear();

                this->text = text;
                std::string::const_iterator c;

                float x = 300.0;
                float y = -340.0;

                for (c = text.begin(); c != text.end(); c++)
                {
                    Character &ch = _characters[*c];

                    float xpos = x + ch.Size.x;
                    float ypos = y - (ch.Bearing.y);

                    float w = ch.Size.x;
                    float h = ch.Size.y;

                    auto g = std::make_shared<Sprite>(ch.bitmap);

                    g->position = glm::vec3(xpos, ypos, 0.0);
                    g->textureId = 2;
                    g->scale = glm::vec3(w, h, 0.0);
                    g->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
                    g->zIndex = 999;

                    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                    x += (ch.Advance >> 6) * 2.0f; // bitshift by 6 to get value in pixels (2^6 = 64)
                    this->children.push_back(g);
                }
            }

        private:
            FT_Library ft;

            FT_Face face;
            std::string text;
            std::map<char, Character> _characters;
            float maxDescent;
        };
    }
}
