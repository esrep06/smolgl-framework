#pragma once 

#include <inttypes.h>
#include <fstream>

#include "cpp-utilz/logger/logger.hpp"

#define GLEW_STATIC
#include <glew/include/GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <glfw/include/GLFW/glfw3.h>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

namespace sm
{
    class shader 
    {
        public: 
            shader(std::string vertex, std::string fragment);
            shader();
            ~shader();

            uint8_t load_and_compile();

            void use();
            void detach();

            void send_float(float val, std::string varName);
            void send_mat4(glm::mat4 mat, std::string name);
            void send_int(int32_t val, std::string name);

            void free();
        private:
            uint32_t m_id;

            std::string m_vertex_string, m_fragment_string, m_vertex_path, m_fragment_path;

            uint8_t m_being_used;
            uint8_t m_was_freed;
            uint8_t m_was_initialized = 0;

    };
}

