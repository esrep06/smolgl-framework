#pragma once

#include <string> 

#define GLEW_STATIC
#include <glew/include/GL/glew.h>

#include "stb_image/stb_image.h"
#include "cpp-utilz/logger/logger.hpp"

namespace sm 
{
    class texture 
    {
        public:
            texture(std::string path);
            ~texture();

            void load();
            uint32_t get_id();
            void bind();
            void detach();
            std::string get_path();
            int32_t get_width();
            int32_t get_height();
            int32_t get_channels();
            uint8_t* get_data();
            void free();
        private:
            int32_t m_width, m_height, m_channels;
            uint32_t m_id;
            std::string m_path;
            uint8_t m_loaded = 0;
            uint8_t* m_data;
    };
}

