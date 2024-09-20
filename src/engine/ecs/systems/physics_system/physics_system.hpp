#pragma once 

#include "../../components/components.hpp"

#include <map>

namespace sm
{
    class physics_system
    {
        public:
            physics_system();

            void add_entity(uint16_t e);

            void remove_entity(uint16_t e);

            void update_physics(sm::transform* t, uint16_t e);

            physics_body* get_component(uint16_t e);

            uint8_t has_entity(uint16_t e);

            std::map<uint16_t, physics_body>* get_entities();
        private:
            std::map<uint16_t, physics_body> m_entities;
    };
}

