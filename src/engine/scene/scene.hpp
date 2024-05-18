#pragma once 

#include <iostream>

#include "../camera/camera.hpp"
#include "../ecs/ecs.hpp"
#include "../shader_pool/shader_pool.hpp"

namespace sm
{
    class scene
    {
        public:
            scene(camera cam);

            void init();
            void render();

            camera* get_camera();
            ecs* get_ecs();
            shader_pool* get_shader_pool();
        private:
            camera m_camera;
            ecs m_ecs;
            shader_pool m_default_shaders;
    };
}

