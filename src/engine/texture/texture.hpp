#pragma once

#include <string> 

#include "../../../include/stb_image/stb_image.h"
#include "../../../include/stb_image/stb_image_write.h"
#include "../../../include/stb_image/stb_image_resize.h"
#include "../../../include/glew/include/GL/glew.h"
#include "../../../include/cpp-utilz/logger/logger.hpp"

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
            void free();
        private:
            int32_t m_width, m_height, m_channels;
            uint32_t m_id;
            std::string m_path;
            uint8_t m_loaded = 0;
    };
}

