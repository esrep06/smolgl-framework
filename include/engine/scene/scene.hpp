#pragma once 

#include "engine/camera/camera.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/shader_pool/shader_pool.hpp"
#include "engine/texture_pool/texture_pool.hpp"
#include "engine/font_pool/font_pool.hpp"
#include "engine/window/window.hpp"

namespace sm
{
    class scene
    {
        public:
            scene(window* win, camera cam);
            ~scene();

            void new_frame();
            void new_physics_frame();
            void render();
            void end_frame();

            camera* get_camera();

            ecs* get_ecs();

            shader_pool* get_shader_pool();
            texture_pool* get_texture_pool();
            font_pool* get_font_pool();

            sm::window* get_window();

            double get_fps();
        private:
            camera m_camera;

            ecs m_ecs;

            shader_pool m_default_shaders;
            texture_pool m_textures;
            font_pool m_default_fonts;

            sm::window* m_window;

            double fps;
            double m_tick_timer = 0.0;
    };
}

