#include "scene.hpp"

namespace sm
{
    scene::scene(camera cam)
        : m_camera(cam), m_ecs(ecs())
    {
        m_default_shaders.add_shader(sm::shader("assets/triangle_vertex.glsl", "assets/triangle_fragment.glsl"), "triangle_shader");
        m_default_shaders.initialize_shaders();
    }

    void scene::init()
    {
        m_ecs.get_sprite_system()->initialize_all();
    }

    // Render all scene entities, update camera matrices 
    void scene::render()
    {
        m_default_shaders.retrieve_shader("triangle_shader")->use();

        for (auto it = m_ecs.get_sprite_system()->get_entities()->begin(); it !=  m_ecs.get_sprite_system()->get_entities()->end(); it++)
        {
            if (m_ecs.get_transform_system()->has_entity(it->first))
                m_ecs.get_transform_system()->update_transform(m_default_shaders.retrieve_shader("triangle_shader"), it->first);

            if (!it->second.was_initialized)
                m_ecs.get_sprite_system()->initialize(it->first);

            m_ecs.get_sprite_system()->draw(it->first);
        }

        m_camera.update();

        m_default_shaders.retrieve_shader("triangle_shader")->send_mat4(m_camera.get_view(), "view");
        m_default_shaders.retrieve_shader("triangle_shader")->send_mat4(m_camera.get_proj(), "projection");

        m_default_shaders.retrieve_shader("triangle_shader")->detach();
    }

    camera* scene::get_camera()
    { return &m_camera; }

    ecs* scene::get_ecs()
    { return &m_ecs; }

    shader_pool* scene::get_shader_pool()
    { return &m_default_shaders; }
}

