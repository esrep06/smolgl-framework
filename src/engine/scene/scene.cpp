#include "scene.hpp"

namespace sm
{
    scene::scene(window* win, camera cam)
        : m_camera(cam), m_ecs(ecs()), m_window(win)
    {}

    void scene::init()
    {
        m_ecs.get_sprite_system()->initialize_all();

        m_default_shaders.add_shader("assets/default_vertex.glsl", "assets/default_fragment.glsl", "default_shader");
        m_default_shaders.add_shader("assets/default_textured_vertex.glsl", "assets/default_textured_fragment.glsl", "default_textured_shader");
    }

    void scene::render()
    {
        // Update camera matrices
        m_camera.update();

        // Send camera matrices to all shaders
        sm::shader* default_shader = m_default_shaders.retrieve_shader("default_shader");
        sm::shader* default_texture_shader = m_default_shaders.retrieve_shader("default_textured_shader");

        // Send camera matrices to shaders 
        m_camera.send_matrices(default_shader, "projection", "view");
        m_camera.send_matrices(default_texture_shader, "projection", "view");

        for (auto it = m_ecs.get_sprite_system()->get_entities()->begin(); it !=  m_ecs.get_sprite_system()->get_entities()->end(); it++)
        {
            // Update entity transforms and send to shaders
            if (it->second.shader != nullptr)
                m_ecs.get_transform_system()->update_transform(it->second.shader, it->first);
            else 
            {
                m_ecs.get_transform_system()->update_transform(m_default_shaders.retrieve_shader("default_shader"), it->first);
                it->second.shader = m_default_shaders.retrieve_shader("default_shader");
            }

            // Initialize if it hasnt 
            if (!it->second.was_initialized)
                m_ecs.get_sprite_system()->initialize(it->first);

            if (m_ecs.get_sprite_system()->get_component(it->first)->shader == nullptr)
                default_shader->use();

            m_ecs.get_sprite_system()->draw(it->first);

            if (m_ecs.get_sprite_system()->get_component(it->first)->shader == nullptr)
                default_shader->detach();
        }

        for (auto it = m_ecs.get_textured_sprite_system()->get_entities()->begin(); it !=  m_ecs.get_textured_sprite_system()->get_entities()->end(); it++)
        {
            // Update entity transforms and send to shaders
            if (it->second.shader != nullptr)
                m_ecs.get_transform_system()->update_transform(it->second.shader, it->first);
            else 
            {
                m_ecs.get_transform_system()->update_transform(m_default_shaders.retrieve_shader("default_textured_shader"), it->first);
                it->second.shader = m_default_shaders.retrieve_shader("default_textured_shader");
            }

            // Initialize if it hasnt 
            if (!it->second.was_initialized)
                m_ecs.get_textured_sprite_system()->initialize(it->first);

            if (m_ecs.get_textured_sprite_system()->get_component(it->first)->shader == nullptr)
                default_texture_shader->use();

            m_ecs.get_textured_sprite_system()->draw(it->first);

            if (m_ecs.get_textured_sprite_system()->get_component(it->first)->shader == nullptr)
                default_texture_shader->detach();
        }

        m_ecs.clear_remove_queue();
    }

    scene::~scene()
    {
        /* m_ecs.free(); */
        /* m_textures.delete_textures(); */
    }

    camera* scene::get_camera()
    { return &m_camera; }

    ecs* scene::get_ecs()
    { return &m_ecs; }

    shader_pool* scene::get_shader_pool()
    { return &m_default_shaders; }

    texture_pool* scene::get_texture_pool()
    { return &m_textures; }

    sm::window* scene::get_window()
    { return m_window; }
}

