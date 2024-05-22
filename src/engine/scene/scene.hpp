#pragma once 

#include <iostream>

#include "../camera/camera.hpp"
#include "../ecs/ecs.hpp"
#include "../shader_pool/shader_pool.hpp"
#include "../texture_pool/texture_pool.hpp"
#include "../utils/time/time.hpp"
#include "../window/window.hpp"

namespace sm
{
    class scene
    {
        public:
            scene(window* win, camera cam);
            ~scene();

            void init();
            void render();

            camera* get_camera();

            ecs* get_ecs();

            shader_pool* get_shader_pool();
            texture_pool* get_texture_pool();

            sm::window* get_window();
        private:
            camera m_camera;

            ecs m_ecs;

            shader_pool m_default_shaders;
            texture_pool m_textures;

            sm::window* m_window;
    };
}

