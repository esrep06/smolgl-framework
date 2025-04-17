#pragma once 

#include <map>
#include "../../../include/cpp-utilz/logger/logger.hpp"
#include "../shader/shader.hpp"

#include "../font/font.hpp"

namespace sm
{
    class font_pool
    {
        public:
            font_pool();

            void add_font(sm::shader* shader, std::string path, float scale, std::string identifier);

            font* retrieve_font(std::string identifier);

            std::map<std::string, font>* get_pool();
        private:
            std::map<std::string, font> m_pool;
    };
}

