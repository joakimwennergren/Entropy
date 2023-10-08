#include "label.hpp"

using namespace Symbios::Text;

Label::Label()
{

	// All of this is temp code @todo
    FT_Init_FreeType(&ft);

    FT_New_Face(ft, (Filesystem::GetProjectBasePath() + "/PTSerif-Regular.ttf").c_str(), 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 48);

    for (uint8_t c = 31; c < 128; c++)
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
                auto g = std::make_shared<Sprite>(face->glyph->bitmap);
                g->textureId = 2;
                g->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
                g->zIndex = 999;

                Character character = {
                    g,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x,
                    face->glyph->metrics.height,
                };
                _characters.insert(std::pair<char, Character>(c, character));
                //this->children.push_back(g);
            }
        }
    }
}

Label::~Label()
{

}
