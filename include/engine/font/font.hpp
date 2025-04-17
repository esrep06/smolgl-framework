#pragma once 

#include <iostream>
#include <inttypes.h>
#include <map>

#define GLEW_STATIC
#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>

#include "../../../include/cpp-utilz/color/color.hpp"
#include "../../../include/cpp-utilz/logger/logger.hpp"
#include "../../../include/cpp-utilz/math/vector2.hpp"

#include "../../../include/engine/gl/gl_layer.hpp"
#include "../../../include/engine/shader/shader.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

namespace sm
{
    class font
    {
        public:
            font(std::string path, uint32_t height);

            struct character 
            {
                uint32_t id;
                glm::ivec2 size;
                glm::ivec2 bearing;
                uint32_t advance;
            };

            std::map<char, character> characters;
            
            void load_characters();
            void render_text(shader* shader, glm::mat4 projection, std::string text, float x, float y, float scale, utilz::rgba_color color);
        private: 
            std::string m_font_path;
            sm::gl_layer::vao m_vao;
            sm::gl_layer::vbo m_vbo;
            int32_t m_ascent = 0;
    };
}

