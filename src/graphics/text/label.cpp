#include "label.hpp"

using namespace Symbios::Text;

Label::Label(std::shared_ptr<Context> context)
{
	// All of this is temp code @todo

    FT_Init_FreeType(&ft);

    FT_New_Face(ft, "/Users/joakim/Desktop/Symbios/resources/fonts/lato/Lato-Regular.ttf", 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 32);

    for (uint8_t c = 0; c < 128; c++)
    {
        auto glyph_index = FT_Get_Char_Index(face, c);

        if (glyph_index != 0)
        {
            auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

            if (error)
            {
                throw std::runtime_error("failed to load glyph");
            }

            FT_GlyphSlot glyphSlot = face->glyph;

            error = FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);
            if (error)
            {
                throw std::runtime_error("failed to render glyph");
            }

            if (glyphSlot->bitmap.width != 0)
            {
                auto g = new Quad(context);
                g->position = glm::vec3(200.0, -500.0, 0.0);
                g->textureId = 2;
                g->texture->CreateTextureImageFromBuffer(face->glyph->bitmap);

                Character character = {
                    g,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x,
                    0
                };
                
                _characters.insert(std::pair<char, Character>(c, character));
            }
        }
    }
}

Label::~Label()
{

}