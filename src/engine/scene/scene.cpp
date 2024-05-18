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
        //m_ecs.get_sprite_system()->initialize();
    }

    // Render all scene entities, update camera matrices 
    void scene::render()
    {
        m_ecs.get_sprite_system()->draw();
        m_ecs.get_transform_system()->update_transform(m_default_shaders.retrieve_shader("triangle_shader"));
        m_camera.update();
    }

    camera* scene::get_camera()
    { return &m_camera; }

    ecs* scene::get_ecs()
    { return &m_ecs; }

    shader_pool* scene::get_shader_pool()
    { return &m_default_shaders; }
}

