#pragma once 

#include "../../components/components.hpp"
#include "../../../../../include/glew/include/GL/glew.h"
#include "../../../../../include/cpp-utilz/logger/logger.hpp"

#include <map>
#include <format>

namespace sm 
{
    class sprite_system 
    {
        public:
            sprite_system();

            void add_entity(uint16_t e);
            void remove_entity(uint16_t e);
            
            void draw();
       
            std::map<uint16_t, sprite>* get_entities();

            uint8_t has_entity(uint16_t e);
        private:
            std::map<uint16_t, sprite> m_entities;
            void initialize(uint16_t e);
    };
}

