#pragma once

#include <iostream>
#include <inttypes.h>

#include "../../../include/glew/include/GL/glew.h"
#include "../../../include/glfw/include/GLFW/glfw3.h"

#include "../input/input.hpp"

#include "../../../include/cpp-utilz/color/color.hpp"
#include "../../../include/cpp-utilz/logger/logger.hpp"
#include "../../../include/cpp-utilz/math/vector2.hpp"

namespace sm
{
    class window 
    {
        public:
            // Constructor creates the window, initializes OpenGL context and GLEW context
            window(std::string title, uint16_t width, uint16_t height, utilz::rgba_color color);
            window();
            ~window();

            void clear();
            void swap_buffers();

            GLFWwindow* get_context();
            utilz::rgba_color get_color();
            utilz::vector2 get_resolution();
            utilz::vector2f get_center();

            void set_resolution(int32_t width, int32_t height);
            void set_color(utilz::rgba_color color);
        private:
            GLFWwindow* m_context;

            uint16_t m_width, m_height;

            std::string m_title;

            utilz::rgba_color m_color;

            static void window_size_callback(GLFWwindow* window, int width, int height);
    };
}

