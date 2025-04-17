#include "engine/font/font.hpp"

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
}

namespace sm
{
    FT_Library m_ft;
    FT_Face m_face;

    font::font(std::string path, uint32_t height)
    {
        if (FT_Init_FreeType(&m_ft))
        {
            utilz::logger::log("Could not init FreeType library\n", utilz::logger::ERROR);
            return;
        }

        if (FT_New_Face(m_ft, path.c_str(), 0, &m_face))
        {
            utilz::logger::log("Failed to load font...\n", utilz::logger::ERROR);
            return;
        }

        // Calulates the width dynamically with the given height
        FT_Set_Pixel_Sizes(m_face, 0, height);

        m_ascent = m_face->size->metrics.ascender >> 6;

        m_font_path = path;

        m_vao.generate();
        m_vao.bind();

        m_vbo.generate();
        m_vbo.bind();

        m_vbo.send_data(sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        sm::gl_layer::enable_attrib_ptr(0, 4, GL_FLOAT, 4 * sizeof(float), 0);
        
        m_vbo.detach();
        m_vao.detach();
    }

    void font::load_characters()
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
            {
                utilz::logger::log("Failed to load Glypth\n", utilz::logger::ERROR);
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    m_face->glyph->bitmap.width,
                    m_face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    m_face->glyph->bitmap.buffer
                    );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            character character = {
                texture, 
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                m_face->glyph->advance.x
            };
            characters.insert({c, character});
        }

        FT_Done_Face(m_face);
        FT_Done_FreeType(m_ft);
    }

    void font::render_text(shader* shader, glm::mat4 projection, std::string text, float x, float y, float scale, utilz::rgba_color color)
    {
        shader->send_vec3((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, "textColor");
        shader->send_mat4(projection, "projection");
        shader->use();
        // activate corresponding render state	
        glActiveTexture(GL_TEXTURE0);
        m_vao.bind();


        float baseline_y = y + m_ascent * scale;

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            character ch = characters[*c];


            float xpos = x + ch.bearing.x * scale;
            float ypos = baseline_y - ch.bearing.y * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 1.0f },            
                { xpos,     ypos,       0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 0.0f },

                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 0.0f },
                { xpos + w, ypos + h,   1.0f, 1.0f }           
            };

            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.id);
            // update content of VBO memory
            m_vbo.bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            // m_vbo.update_data(0, sizeof(vertices), vertices);
            m_vbo.detach();

            sm::gl_layer::enable_attrib_array(0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
            sm::gl_layer::disable_attrib_array(0);
        }
        shader->detach();
        m_vao.detach();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

