#pragma once

#include <iostream>
#include <inttypes.h>

#include "../../../include/glew/include/GL/glew.h"
#include "../../../include/glfw/include/GLFW/glfw3.h"
#include "../include/imgui/imgui.h"
#include "../include/imgui/backends/imgui_impl_glfw.h"
#include "../include/imgui/backends/imgui_impl_opengl3.h"

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

            void new_frame();
            void clear();
            void swap_buffers();

            GLFWwindow* get_context();
            utilz::rgba_color get_color();
            utilz::vector2 get_resolution();
        private:
            GLFWwindow* m_context;

            uint16_t m_width, m_height;

            std::string m_title;

            utilz::rgba_color m_color;
    };
}

