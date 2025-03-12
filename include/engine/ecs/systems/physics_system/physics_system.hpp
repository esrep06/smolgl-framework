#pragma once 

#include "../../components/components.hpp"
#include "cpp-utilz/math/vector2.hpp"
#include <map>
#include <format>

#define WORLD_GRAVITY 8000

#define PHYSICS_TICK 1.0f / 120.0f

namespace sm
{
    class physics_system
    {
        public:
            physics_system();

            void add_entity(uint16_t e);

            void remove_entity(uint16_t e);

            void update_physics(sm::transform* t, uint16_t e);
            static void dynamic_body_move(utilz::vector2f delta, transform* t, physics_body* body);
            static void dynamic_body_add_force(utilz::vector2f force, transform* t, physics_body* body);

            physics_body* get_component(uint16_t e);

            uint8_t has_entity(uint16_t e);

            std::map<uint16_t, physics_body>* get_entities();
        private:
            std::map<uint16_t, physics_body> m_entities;
    };
}

