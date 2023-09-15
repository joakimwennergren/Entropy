/**
 * @file easing.hpp
 * @author Joakim Wennergren (joakim.wennergren@databeams.se)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/ftglyph.h>

#include <glm/glm.hpp>

#include "renderable.hpp"
#include "sprite.hpp"

using namespace Symbios::Renderables;
using namespace Symbios::Graphics::Primitives;

namespace Symbios
{
    namespace Text
    {
        struct Character
        {
            Sprite *glyph;
            glm::ivec2 Size;
            glm::ivec2 Bearing; 
            FT_Pos Advance;     
            FT_Pos YAdvance;
        };

        class Label : public Renderable
        {
        public:
            Label(std::shared_ptr<Context> context, std::string text);
            ~Label();   
            std::vector<Sprite *> sprites;

        private:
            FT_Library ft;
            FT_Face face;  
            std::map<char, Character> _characters;
            float maxDescent;
        };
    }
}