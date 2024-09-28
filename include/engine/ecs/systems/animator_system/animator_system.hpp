#pragma once

#pragma once 

#include "cpp-utilz/logger/logger.hpp"
#include "../../../texture/texture.hpp"
#include "../../components/components.hpp"

#include <map>
#include <format>

namespace sm
{
    class animator_system
    {
        public:
            animator_system();

            void add_entity(uint16_t e);

            void remove_entity(uint16_t e);

            animator* get_component(uint16_t e);

            void update_animation(uint16_t e, textured_sprite* sprite);

            static void enable_animation(animator* anim, std::string);

            static int32_t add_animation(animator* anim, animation a);

            uint8_t has_entity(uint16_t e);

            std::map<uint16_t, animator>* get_entities();
        private:
            std::map<uint16_t, animator> m_entities;
    };
}

