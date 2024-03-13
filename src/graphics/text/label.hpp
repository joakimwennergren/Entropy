#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <graphics/primitives/2d/sprite.hpp>
#include <filesystem/filesystem.hpp>
#include <servicelocators/servicelocator.hpp>
#include <graphics/text/font.hpp>

using namespace Entropy::Graphics::Primitives;
using namespace Entropy::ServiceLocators;
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

        class Label
        {

        public:
            std::string text;
            Label(std::shared_ptr<ServiceLocator> serviceLocator, std::shared_ptr<Font> font);
            void Test(){};
            std::vector<std::shared_ptr<Sprite>> sprites;

            inline void SetPosition(float x, float y)
            {
                if (this->text.length() == 0)
                    return;

                for (unsigned int i = 0; i < text.size(); i++)
                {
                    float xpos = x;
                    float ypos = y;

                    auto yAdvance = (_font->glyphs[text[i]].glyphslot->bitmap.rows - _font->glyphs[text[i]].glyphslot->bitmap_top) + ypos;

                    // // auto g = this->children[i];
                    // g->position = glm::vec3(xpos, yAdvance, 1.0);

                    // x += (_font->glyphs[text[i]].glyphslot->advance.x >> 6);
                }
            }

            inline void SetText(std::string text)
            {
                // this->children.clear();

                // this->text = text;
                // float x = position.x;
                // float y = position.y;

                for (unsigned int i = 0; i < text.size(); i++)
                {
                    // if (text[i] == 32)
                    // {
                    //     x += 20;
                    //     continue;
                    // }

                    // float xpos = x; //_font->glyphs[text[i]].glyphslot->bitmap.width;
                    // float ypos = y;

                    // float w = _font->glyphs[text[i]].glyphslot->bitmap.width / 2.0;
                    // float h = _font->glyphs[text[i]].glyphslot->bitmap.rows / 2.0;

                    // auto g = std::make_shared<Sprite>(_serviceLocator, _font->glyphs[text[i]].glyphslot->bitmap);

                    // auto yAdvance = (_font->glyphs[text[i]].glyphslot->bitmap.rows - _font->glyphs[text[i]].glyphslot->bitmap_top) + ypos;

                    // g->position = glm::vec3(xpos, yAdvance, 1.0);
                    // g->textureId = 2;
                    // g->type = 3;
                    // g->scale = glm::vec3(w, h, 0.0);
                    // g->color = this->color;
                    // g->zIndex = 999;

                    // // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                    // x += (_font->glyphs[text[i]].glyphslot->advance.x >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
                    // this->children.push_back(g);
                }
            }

        private:
            FT_Library ft;
            FT_Face face;
            std::shared_ptr<ServiceLocator> _serviceLocator;
            std::map<unsigned char, Character> _characters;
            std::shared_ptr<Font> _font;
        };
    }
}
