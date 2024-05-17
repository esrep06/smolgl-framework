#pragma once

#include <iostream>
#include <inttypes.h>

#include "../../../include/glew/include/GL/glew.h"
#include "../../../include/glfw/include/GLFW/glfw3.h"

#include "../input/input.hpp"

#include "../../../include/cpp-utilz/color/color.hpp"
#include "../../../include/cpp-utilz/logger/logger.hpp"

namespace kt
{
    class window 
    {
        public:
            // Constructor creates the window, initializes OpenGL context and GLEW context
            window(std::string title, uint16_t width, uint16_t height, utilz::rgba_color color);
            window();
            ~window();

            GLFWwindow* get_context();
            utilz::rgba_color get_color();
        private:
            GLFWwindow* m_context;

            uint16_t m_width, m_height;

            std::string m_title;

            utilz::rgba_color m_color;
    };
}

