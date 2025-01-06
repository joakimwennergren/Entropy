#pragma once

#include <map>

#include <ft2build.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <graphics/primitives/2d/sprite.hpp>
#include <filesystem/filesystem.hpp>

using namespace Entropy::Graphics::Primitives;

namespace Entropy
{
    namespace Text
    {

        class Glyph
        {
        public:
            FT_GlyphSlot glyphslot;
        };

        class Font
        {
        public:
            Font(std::string path);
            std::map<uint8_t, Glyph> glyphs;

        private:
            FT_Library ft;
            FT_Face face;
        };
    }
}
