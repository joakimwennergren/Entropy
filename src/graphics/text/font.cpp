#include "font.hpp"

using namespace Entropy::Text;

Font::Font(std::string path)
{
    // All of this is temp code @todo
    FT_Init_FreeType(&ft);

    for (uint8_t n = 32; n < 128; n++)
    {
        FT_New_Face(ft, path.c_str(), 0, &face);

        FT_Set_Pixel_Sizes(face, 0, 16);

        auto glyph_index = FT_Get_Char_Index(face, n);
        auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);

        auto glyph = Glyph{
            .glyphslot = face->glyph};

        glyphs.insert(std::pair<uint8_t, Glyph>(n, glyph));
    }
}