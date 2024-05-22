#pragma once 

#include "../../components/components.hpp"
#include "../../../../../include/glew/include/GL/glew.h"
#include "../../../../../include/cpp-utilz/logger/logger.hpp"

#include <map>
#include <format>

namespace sm 
{
    class textured_sprite_system 
    {
        public:
            textured_sprite_system();

            void add_entity(uint16_t e);
            void remove_entity(uint16_t e);
            
            void draw(uint16_t e);
       
            std::map<uint16_t, textured_sprite>* get_entities();
            
            textured_sprite* get_component(uint16_t e);

            void initialize_all();
            void initialize(uint16_t e);

            uint8_t has_entity(uint16_t e);
        private:
            std::map<uint16_t, textured_sprite> m_entities;
    };
}

