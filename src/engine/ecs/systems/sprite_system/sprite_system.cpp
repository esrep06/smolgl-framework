#include "sprite_system.hpp"
#include "cpp-utilz/math/vector3.hpp"

namespace sm 
{
    sprite_system::sprite_system() 
    {}

    void sprite_system::add_entity(uint16_t e)
    {

        if (has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' already exists in transform system\n", e), utilz::logger::WARNING);
            return;
        }

        m_entities.insert(std::pair<uint16_t, sprite>(e, sprite()));

        initialize(e);
    }

    void sprite_system::remove_entity(uint16_t e)
    {
        if (!has_entity(e)) 
        {
            utilz::logger::log(std::format("Entity '{}' does not exist in transform system\n", e), utilz::logger::ERROR);
            return;
        }

        m_entities.erase(e);
    }

    void sprite_system::initialize(uint16_t e)
    {
        sprite* spr = &m_entities.at(e);
        
        if (spr->was_initialized) return;
        
        spr->was_initialized = 1;

        spr->vertices[0].position = utilz::vector3f(0.0f, 0.0f, 1.0f);
        spr->vertices[1].position = utilz::vector3f(1.0f, 0.0f, 1.0f);
        spr->vertices[2].position = utilz::vector3f(0.0f, 1.0f, 1.0f);
        spr->vertices[3].position = utilz::vector3f(1.0f, 1.0f, 1.0f);

        spr->indices[0] = 0;
        spr->indices[1] = 1;
        spr->indices[2] = 2;

        spr->indices[3] = 2;
        spr->indices[4] = 3;
        spr->indices[5] = 1;

        spr->vao = gl_layer::vao();
        spr->vbo = gl_layer::vbo();
        spr->ebo = gl_layer::ebo();

        spr->vbo.send_data(sizeof(spr->vertices), spr->vertices, GL_STATIC_DRAW);
        spr->ebo.send_data(sizeof(spr->indices), spr->indices, GL_STATIC_DRAW);

        sm::gl_layer::enable_attrib_ptr(0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
        gl_layer::enable_attrib_array(0);

        spr->vbo.detach();
        spr->vao.detach();
        spr->ebo.detach();
    }

   
    void sprite_system::draw()
    {
        for (auto it = m_entities.begin(); it != m_entities.end(); it++)
        {
            it->second.vbo.bind();
            it->second.vao.bind();
            it->second.ebo.bind();

            gl_layer::enable_attrib_array(0);

            gl_layer::draw_indexed(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            gl_layer::disable_attrib_array(0);

            it->second.vbo.detach();
            it->second.vao.detach();
            it->second.ebo.detach();
        }
    }

    uint8_t sprite_system::has_entity(uint16_t e)
    { return m_entities.find(e) != m_entities.end(); }

    std::map<uint16_t, sprite>* sprite_system::get_entities()
    { return &m_entities; }
}

