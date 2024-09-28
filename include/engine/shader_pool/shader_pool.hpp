#pragma once 

#include <map>
#include "../../../include/cpp-utilz/logger/logger.hpp"

#include "../shader/shader.hpp"

namespace sm
{
    class shader_pool
    {
        public:
            shader_pool();

            void add_shader(std::string vertex, std::string fragment, std::string identifier);
            void initialize_shaders();

            shader* retrieve_shader(std::string identifier);
            void delete_shader(std::string identifier);

            std::map<std::string, shader>* get_pool();
        private:
            std::map<std::string, shader> m_pool;
    };
}

