#include "engine/scene/scene.hpp"
#include "engine/ecs/systems/physics_system/physics_system.hpp"

    namespace sm
    {
        scene::scene(window* win, camera cam)
            : m_camera(cam), m_ecs(ecs()), m_window(win)
        {
            // Compile our default shaders
            m_default_shaders.add_shader("assets/shaders/default_vertex.glsl", 
                    "assets/shaders/default_fragment.glsl", "default_shader");
            m_default_shaders.add_shader("assets/shaders/default_textured_vertex.glsl", 
                    "assets/shaders/default_textured_fragment.glsl", "default_textured_shader");
        }

        void scene::new_frame()
        {
            time::calculate_update();
            glfwPollEvents();

            // Delete all entities marked for deletion before starting new frame 
            m_ecs.clear_remove_queue();

            m_window->clear(); 
        }

        void scene::new_physics_frame() 
        {
            sm::physics_system* physics_system = m_ecs.get_physics_system();
            sm::transform_system* transform_system = m_ecs.get_transform_system();

            if (m_tick_timer >= PHYSICS_TICK)
            {
                for (auto it = physics_system->get_entities()->begin(); it != physics_system->get_entities()->end(); it++) 
                {
                    if (transform_system->has_entity(it->first))
                        physics_system->update_physics(transform_system->get_component(it->first), it->first);    
                }
                m_tick_timer = 0.0;
            }
        }


        void scene::render()
        {
            m_camera.bounds = m_window->get_resolution();

            // Update camera matrices
            m_camera.update();

            // Send camera matrices to shaders 
            shader* default_shader = m_default_shaders.retrieve_shader("default_shader");
            shader* default_texture_shader = m_default_shaders.retrieve_shader("default_textured_shader");
            m_camera.send_matrices(default_shader, "projection", "view");
            m_camera.send_matrices(default_texture_shader, "projection", "view");

            sm::behavior_system* behavior_system = m_ecs.get_behavior_system();
            sm::transform_system* transform_system = m_ecs.get_transform_system();
            sm::sprite_system* sprite_system = m_ecs.get_sprite_system();
            sm::textured_sprite_system* textured_sprite_system = m_ecs.get_textured_sprite_system();
            sm::animator_system* animator_system = m_ecs.get_animator_system();

            for (auto it = behavior_system->get_entities()->begin(); it != behavior_system->get_entities()->end(); it++)
            {
                behavior_system->start(it->first);
                behavior_system->update(it->first);            
            }

            for (auto it = sprite_system->get_entities()->begin(); it !=  sprite_system->get_entities()->end(); it++)
            {
                // Update entity transforms and send to shaders
                if (it->second.shader != nullptr)
                    transform_system->update_transform(it->second.shader, it->first);
                else 
                {
                    transform_system->update_transform(m_default_shaders.retrieve_shader("default_shader"), it->first);
                    it->second.shader = m_default_shaders.retrieve_shader("default_shader");
                }

                // Initialize if it hasnt 
                if (!it->second.was_initialized)
                    sprite_system->initialize(it->first);

                if (sprite_system->get_component(it->first)->shader == nullptr)
                    default_shader->use();
                else
                    sprite_system->get_component(it->first)->shader->use();

                sprite_system->draw(it->first);

                if (sprite_system->get_component(it->first)->shader == nullptr)
                    default_shader->detach();
                else 
                    sprite_system->get_component(it->first)->shader->detach();
            }

            for (auto it = textured_sprite_system->get_entities()->begin(); it !=  textured_sprite_system->get_entities()->end(); it++)
            {
                // Update entity transforms and send to shaders
                if (it->second.shader != nullptr)
                    transform_system->update_transform(it->second.shader, it->first);
                else 
                {
                    transform_system->update_transform(m_default_shaders.retrieve_shader("default_textured_shader"), it->first);
                    it->second.shader = m_default_shaders.retrieve_shader("default_textured_shader");
            }

            // Initialize if it hasnt 
            if (!it->second.was_initialized)
                textured_sprite_system->initialize(it->first);

            if  (textured_sprite_system->get_component(it->first)->shader == nullptr)
                default_texture_shader->use();

            textured_sprite_system->draw(it->first);

            if  (textured_sprite_system->get_component(it->first)->shader == nullptr)
                default_texture_shader->detach();

            // If the texture sprite has an animator 

            if (animator_system->has_entity(it->first))
                animator_system->update_animation(it->first, &it->second);
        }
    }

    void scene::end_frame()
    {
        m_window->swap_buffers();
        m_ecs.clear_remove_queue();
        input::end_frame();
        fps = 1.0 / (double)sm::time::delta_time();
        m_tick_timer += sm::time::delta_time();
    }

    scene::~scene()
    {}

    camera* scene::get_camera()
    { return &m_camera; }

    ecs* scene::get_ecs()
    { return &m_ecs; }

    shader_pool* scene::get_shader_pool()
    { return &m_default_shaders; }

    texture_pool* scene::get_texture_pool()
    { return &m_textures; }

    window* scene::get_window()
    { return m_window; }

    double scene::get_fps() 
    { return fps; } 
}

