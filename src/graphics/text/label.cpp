#include "label.hpp"

using namespace Symbios::Text;

Label::Label(std::string text)
{

    // Store vulkan ctx
    auto context = Global::GetInstance()->GetVulkanContext();

	// All of this is temp code @todo

    FT_Init_FreeType(&ft);

    FT_New_Face(ft, (Filesystem::GetProjectBasePath() + "/Lato-Regular.ttf").c_str(), 0, &face);

    FT_Set_Pixel_Sizes(face, 0, 32);

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
                auto g = new Sprite();
                g->position = glm::vec3(200.0, 0.0, 0.0);
                g->textureId = 2;
                g->texture->CreateTextureImageFromBuffer(face->glyph->bitmap);

                Character character = {
                    g,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x,
                    face->glyph->metrics.height,
                };

                _characters.insert(std::pair<char, Character>(c, character));
            }
        }
    }

    float x = 300.0, y = -300.0;
    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = _characters[*c];

        float xpos = x + ch.Size.x;
        float ypos = y - (ch.Bearing.y);

        float w = ch.Size.x;
        float h = ch.Size.y;

        auto g = new Sprite();
        g->position = glm::vec3(xpos, ypos, 0.0);
        g->textureId = 2;
        g->scale = glm::vec3(w, h, 0.0);
        g->texture = ch.glyph->texture;

        g->UpdateImage();

        PLOG_ERROR << ypos;

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * 2.0f; // bitshift by 6 to get value in pixels (2^6 = 64)
        sprites.push_back(g);
    }
}

Label::~Label()
{

}
