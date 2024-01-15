#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <graphics/primitives/2d/sprite.hpp>
#include <filesystem/filesystem.hpp>
#include <renderables/renderable.hpp>
#include <servicelocators/servicelocator.hpp>
#include <scenegraphs/scenegraph.hpp>
#include <graphics/text/font.hpp>

using namespace Entropy::Renderables;
using namespace Entropy::Graphics::Primitives;
using namespace Entropy::ServiceLocators;
using namespace Entropy::SceneGraphs;
using namespace Entropy::Text;

namespace Entropy
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
            Label(std::shared_ptr<ServiceLocator> serviceLocator, std::shared_ptr<Font> font);
            ~Label();
            std::vector<std::shared_ptr<Sprite>> sprites;

            inline void SetPosition(float x, float y)
            {
                if (this->text.length() == 0)
                    return;

                std::string::const_iterator c;

                for (c = text.begin(); c != text.end(); c++)
                {
                    // Character &ch = _characters[*c];

                    // float xpos = x + ch.Size.x;
                    // float ypos = y - (ch.Bearing.y);

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
                float x = position.x;
                float y = position.y;

                for (unsigned int i = 0; i < text.size(); i++)
                {
                    if (text[i] == 32)
                    {
                        x += 20;
                        continue;
                    }

                    float xpos = x + _font->glyphs[text[i]].glyphslot->bitmap.width;
                    float ypos = y - (_font->glyphs[text[i]].glyphslot->bitmap_top);

                    float w = _font->glyphs[text[i]].glyphslot->bitmap.width;
                    float h = _font->glyphs[text[i]].glyphslot->bitmap.rows;

                    auto g = std::make_shared<Sprite>(_serviceLocator, _font->glyphs[text[i]].glyphslot->bitmap);

                    auto yAdvance = _font->glyphs[text[i]].glyphslot->bitmap_top - h;

                    g->position = glm::vec3(xpos, ypos - yAdvance, 0.0);
                    g->textureId = 2;
                    g->scale = glm::vec3(w, h, 0.0);
                    g->color = this->color;
                    g->zIndex = 999;

                    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                    x += (_font->glyphs[text[i]].glyphslot->advance.x >> 6) * 2.0f; // bitshift by 6 to get value in pixels (2^6 = 64)
                    this->children.push_back(g);
                }
            }

        private:
            FT_Library ft;
            FT_Face face;
            std::string text;
            std::map<unsigned char, Character> _characters;
            std::shared_ptr<ServiceLocator> _serviceLocator;
            std::shared_ptr<Font> _font;
        };
    }
}
