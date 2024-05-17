#pragma once 

#include <inttypes.h>
#include <iostream>
#include <fstream>

#include "../../../include/cpp-utilz/logger/logger.hpp"

#define GLFW_INCLUDE_NONE
#include "../../../include/glew/include/GL/glew.h"
#include "../../../include/glfw/include/GLFW/glfw3.h"

namespace kt
{
    class shader 
    {
        public: 
            shader(std::string vertex, std::string fragment);
            shader();

            uint8_t load_and_compile();

            void use();
            void detach();

            uint8_t send_float(float val, std::string varName);
            void free();
        private:
            uint32_t m_id;

            std::string m_vertex_string, m_fragment_string, m_vertex_path, m_fragment_path;

            uint8_t m_being_used;
            uint8_t m_was_freed;
            uint8_t m_was_initialized;
    };
}

