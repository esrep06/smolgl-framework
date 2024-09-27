#pragma once

#include <string>
#include "cpp-utilz/math/vector2.hpp"
#include "../shader/shader.hpp"

namespace sm 
{
    class camera
    {
        public:
            camera(utilz::vector2f pos, utilz::vector2 bounds);

            void update();

            glm::mat4 get_view();
            glm::mat4 get_proj();

            utilz::vector2f pos;
            utilz::vector2 bounds;

            void send_matrices(shader* shader, std::string p_name, std::string v_name);
        private:
            glm::mat4 m_view, m_proj;
    };
}

