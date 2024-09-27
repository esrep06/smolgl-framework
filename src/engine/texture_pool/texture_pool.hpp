#pragma once

#include <map>
#include "cpp-utilz/logger/logger.hpp"

#include "../texture/texture.hpp"

namespace sm
{
    class texture_pool
    {
        public:
            texture_pool();

            void add_texture(std::string path, std::string identifier);
            void initialize_textures();

            texture* retrieve_texture(std::string identifier);
            void delete_texture(std::string identifier);

            std::map<std::string, texture>* get_pool();
        private:
            std::map<std::string, texture> m_pool;
    };
}
