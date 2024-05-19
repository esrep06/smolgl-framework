#pragma once 

#include "../../../../../include/glm/glm/glm.hpp"
#include "../../../../../include/glm/glm/gtc/matrix_transform.hpp"
#include "../../../../../include/glm/glm/gtc/type_ptr.hpp"

#include "../../../../../include/cpp-utilz/math/vector2.hpp"
#include "../../../../../include/cpp-utilz/logger/logger.hpp"


#include "../../../shader/shader.hpp"
#include "../../components/components.hpp"

#include <map>
#include <format>

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

