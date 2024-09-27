#pragma once 

#include "../../components/components.hpp"
#include "../../../shader/shader.hpp"

#include <format>
#include <map>

namespace sm
{
    class transform_system
    {
        public:
            transform_system();

            void add_entity(uint16_t e);

            void remove_entity(uint16_t e);

            void update_transform(shader* shader, uint16_t e);

            transform* get_component(uint16_t e);

            uint8_t has_entity(uint16_t e);

            std::map<uint16_t, transform>* get_entities();
        private:
            std::map<uint16_t, transform> m_entities;
    };
}

