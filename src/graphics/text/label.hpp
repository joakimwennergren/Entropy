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

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/ftglyph.h>

#include <glm/glm.hpp>

#include "sprite.hpp"
#include "filesystem.hpp"
#include "vulkancontext.hpp"
#include "renderable.hpp"
#include "scenegraph.hpp"

using namespace Symbios::Renderables;
using namespace Symbios::Graphics::Primitives;
using namespace Symbios;

namespace Symbios
{
    namespace Text
    {
        struct Character
        {
            std::shared_ptr<Sprite> glyph;
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
                if(this->text.length() == 0)
                    return;
                
                std::string::const_iterator c;
                int cnt = 0;

                for (c = text.begin(); c != text.end(); c++)
                {
                    Character ch = _characters[*c];

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
                auto sceneGraph = Symbios::Global::SceneGraph::GetInstance();
                
                //this->children.clear();

                this->text = text;
                std::string::const_iterator c;
                
                float x = 300.0;
                float y = -640.0;
                
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
                    g->id = rand() % 9999999999;
                    g->scale = glm::vec3(w, h, 0.0);
                    g->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
                    g->zIndex = 999;
                    g->texture = ch.glyph->texture;
                    g->UpdateImage();

                    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                    x += (ch.Advance >> 6) * 2.0f; // bitshift by 6 to get value in pixels (2^6 = 64)
                    //Symbios::Global::SceneGraph::GetInstance()->renderables.push_back(g);
                    //this->children.push_back(g);
                }
            }
            
            inline void SetZIndex(int index) {this->zIndex = index;};

        private:
            FT_Library ft;
            FT_Face face;  
            std::string text;
            std::map<int, Character> _characters;
            float maxDescent;
        };
    }
}
