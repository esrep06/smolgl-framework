#include "scene.hpp"

namespace sm
{
    scene::scene(window* win, camera cam)
        : m_window(win), m_camera(cam), m_ecs(ecs())
    {}

    void scene::init()
    {
        m_ecs.get_sprite_system()->initialize_all();

        m_default_shaders.add_shader(sm::shader("assets/default_vertex.glsl", "assets/default_fragment.glsl"), "default_shader");

        /* m_default_shaders.initialize_shaders(); */
    }

    void scene::render()
    {
        // Update camera matrices
        m_camera.update();

        // Send camera matrices to all shaders
        sm::shader* default_shader = m_default_shaders.retrieve_shader("default_shader");
        default_shader->send_mat4(m_camera.get_view(), "view");
        default_shader->send_mat4(m_camera.get_proj(), "projection");

        for (auto it = m_ecs.get_sprite_system()->get_entities()->begin(); it !=  m_ecs.get_sprite_system()->get_entities()->end(); it++)
        {
            for (auto shader = m_default_shaders.get_pool()->begin(); shader != m_default_shaders.get_pool()->end(); shader++)
            {
                m_ecs.get_transform_system()->update_transform(m_default_shaders.retrieve_shader(shader->first), it->first);
            }

            // Update entity transforms and send to matrices 

            if (!it->second.was_initialized)
                m_ecs.get_sprite_system()->initialize(it->first);

            if (m_ecs.get_sprite_system()->get_component(it->first)->shader == nullptr)
                default_shader->use();

            m_ecs.get_sprite_system()->draw(it->first);

            if (m_ecs.get_sprite_system()->get_component(it->first)->shader == nullptr)
                default_shader->detach();
        }

        m_ecs.clear_remove_queue();
    }

    camera* scene::get_camera()
    { return &m_camera; }

    ecs* scene::get_ecs()
    { return &m_ecs; }

    shader_pool* scene::get_shader_pool()
    { return &m_default_shaders; }

    sm::window* scene::get_window()
    { return m_window; }
}

