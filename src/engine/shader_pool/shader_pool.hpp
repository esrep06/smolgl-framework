#pragma once 

#include "../shader/shader.hpp"
#include <map>

namespace sm
{
    class shader_pool
    {
        public:
            shader_pool();

            void add_shader(shader value, std::string identifier);
            void initialize_shaders();

            shader* retrieve_shader(std::string identifier);
            void delete_shader(std::string identifier);
            void delete_shaders();

            std::map<std::string, shader>* get_pool();
        private:
            std::map<std::string, shader> m_pool;
    };
}

