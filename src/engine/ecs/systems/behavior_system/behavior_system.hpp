#pragma once 

#include "../../components/components.hpp"
#include "../../../../../include/cpp-utilz/math/vector2.hpp"
#include "../../../../../include/cpp-utilz/logger/logger.hpp"

#include <map>
#include <format>

namespace sm
{
    class behavior_system
    {
        public:
            behavior_system();

            void add_entity(uint16_t e);

            void remove_entity(uint16_t e);

            void start(uint16_t e);
            void update(uint16_t e);

            behavior* get_component(uint16_t e);

            uint8_t has_entity(uint16_t e);

            std::map<uint16_t, behavior>* get_entities();
        private:
            std::map<uint16_t, behavior> m_entities;
    };
}
