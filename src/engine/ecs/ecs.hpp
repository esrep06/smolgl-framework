#pragma once 

#include <inttypes.h>
#include <vector>

#include "components/components.hpp"
#include "../../../include/cpp-utilz/logger/logger.hpp"
#include "systems/transform_system/transform_system.hpp"
#include "systems/sprite_system/sprite_system.hpp"

#define ECS_MAX_ENTITIES 10000

namespace sm 
{
    typedef uint16_t entity;

    class ecs 
    {
        public:
            ecs();

            entity create_entity(component components);

            void remove_entity(entity e);

            uint32_t get_entity_num();

            std::vector<entity>* get_entities();

            void* get_component(entity e, component c);
            void add_components(entity e, component components);

            transform_system* get_transform_system();
            sprite_system* get_sprite_system();

            void clear_remove_queue();
        private:
            uint16_t m_entity_num;
            std::vector<entity> m_entities;
            std::vector<entity> m_remove_queue;

            transform_system m_transform_system;
            sprite_system m_sprite_system;
    };
}

