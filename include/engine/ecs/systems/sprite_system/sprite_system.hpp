#pragma once 

#include "../../components/components.hpp"
#define GLEW_STATIC 
#include <glew/include/GL/glew.h>

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
            
            void draw(uint16_t e);
       
            std::map<uint16_t, sprite>* get_entities();
            
            sprite* get_component(uint16_t e);

            void initialize_all();
            void initialize(uint16_t e);

            uint8_t has_entity(uint16_t e);

            static void update_color(sprite* spr);
        private:
            std::map<uint16_t, sprite> m_entities;
    };
}

